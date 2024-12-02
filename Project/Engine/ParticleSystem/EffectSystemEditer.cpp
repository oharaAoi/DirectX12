#include "EffectSystemEditer.h"
#include "Engine/Utilities/DrawUtils.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"

EffectSystemEditer::EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	Init(renderTarget, descriptorHeaps, dxCommands, device);
}

EffectSystemEditer::~EffectSystemEditer() {}

void EffectSystemEditer::Finalize() {
	depthStencilResource_.Reset();
	effectList_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Init(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {

	renderTarget_ = renderTarget;
	descriptorHeaps_ = descriptorHeaps;
	dxCommands_ = dxCommands;

	// -------------------------------------------------
	// ↓ 深度バッファの作成
	// -------------------------------------------------
	depthStencilResource_ = CreateDepthStencilTextureResource(device, kClientWidth_, kClientHeight_);
	// DSVの生成
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(depthStencilResource_.Get(), &desc, descriptorHeaps_->GetDescriptorHandle(TYPE_DSV).handleCPU);

	// -------------------------------------------------
	// ↓ カメラの初期化
	// -------------------------------------------------
	effectSystemCamera_ = std::make_unique<EffectSystemCamera>();

	particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Update() {
	// カメラの更新
	effectSystemCamera_->Update();

	for (std::list<std::unique_ptr<GpuEffect>>::iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->SetViewProjectionMat(effectSystemCamera_->GetViewMatrix() * effectSystemCamera_->GetProjectionMatrix());
		(*it)->Update();
		++it;
	}
	
	particleField_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Draw() const {
	// Grid線描画
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());

	// 実際にEffectを描画する
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (std::list<std::unique_ptr<GpuEffect>>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->Draw();
		++it;
	}

	// 最後にImGui上でEffectを描画する
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	
	ImGui::Begin("Render Target View");
	ImTextureID textureID = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetOffScreenSRVHandle(RenderTargetType::EffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::Image((void*)textureID, ImVec2(static_cast<float>(640), static_cast<float>(360))); // サイズは適宜調整
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　開始
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Begin() {
	// ここでゲーム描画のRenderTargetからEffect用のRenderTargetに変更する
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	// dsvのポインターを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle.ptr += size_t(descriptorHeaps_->GetDescriptorSize()->GetDSV());
	// RenderTargetを指定する
	renderTarget_->SetRenderTarget(commandList, RenderTargetType::EffectSystem_RenderTarget);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	// RenderTargetをクリアする
	commandList->ClearRenderTargetView(renderTarget_->GetOffScreenHandle(RenderTargetType::EffectSystem_RenderTarget).handleCPU, clearColor, 0, nullptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::End() {
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectの作成をする
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::CreateEffect(const std::string& newName) {
	auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
	newEffect->Init();
	newEffect->SetEffectName(newName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Debug_Gui() {
	if (ImGui::Button("CreateEffect")) {
		uint32_t effectNum = static_cast<uint32_t>(effectList_.size()) + 1;
		std::string defalutName = "effect" + std::to_string(effectNum);
		CreateEffect(defalutName);
	}

	for (std::list<std::unique_ptr<GpuEffect>>::iterator it = effectList_.begin(); it != effectList_.end(); ++it) {
		(*it)->Debug_Gui();
	}

}
#endif // _DEBUG