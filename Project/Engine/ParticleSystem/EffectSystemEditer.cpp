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

	if (effectSystemCamera_->GetIsFocused()) {
		isFocused_ = true;
	} else {
		isFocused_ = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Draw() const {
	ImGui::Begin("Render Target View");

	// Grid線描画
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());


	// 実際にEffectを描画する
	Engine::SetPipeline(PipelineType::AddPipeline);
	for (std::list<std::unique_ptr<GpuEffect>>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->Draw();
		++it;
	}

	// 最後にImGui上でEffectを描画する
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	ImTextureID textureID2 = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetOffScreenSRVHandle(RenderTargetType::EffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::SetCursorPos(ImVec2(20, 30)); // 描画位置を設定
	ImGui::Image((void*)textureID2, ImVec2(640.0f, 360.0f)); // サイズは適宜調整

	// windowの判定
	if (ImGui::IsWindowFocused()) {
		effectSystemCamera_->SetIsFocused(true);
	} else {
		effectSystemCamera_->SetIsFocused(false);
	}

	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　開始
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::PreBegin() {
	// ここでゲーム描画のRenderTargetからEffect用のRenderTargetに変更する
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	// dsvのポインターを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle.ptr += size_t(descriptorHeaps_->GetDescriptorSize()->GetDSV());
	// RenderTargetを指定する
	renderTarget_->SetRenderTarget(commandList, RenderTargetType::PreEffectSystem_RenderTarget);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// RenderTargetをクリアする
	commandList->ClearRenderTargetView(renderTarget_->GetOffScreenHandle(RenderTargetType::PreEffectSystem_RenderTarget).handleCPU, clearColor, 0, nullptr);

	//------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Render Target View");

	// Grid線描画
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());

	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), PreEffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	ImTextureID textureID = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetOffScreenSRVHandle(RenderTargetType::PreEffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::SetCursorPos(ImVec2(20, 30)); // 描画位置を設定
	ImGui::Image((void*)textureID, ImVec2(640.0f, 360.0f)); // サイズは適宜調整

	ImGui::End();
}

void EffectSystemEditer::Begin() {
	// ここでゲーム描画のRenderTargetからEffect用のRenderTargetに変更する
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	// dsvのポインターを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle.ptr += size_t(descriptorHeaps_->GetDescriptorSize()->GetDSV());
	// RenderTargetを指定する
	renderTarget_->SetRenderTarget(commandList, RenderTargetType::EffectSystem_RenderTarget);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// RenderTargetをクリアする
	commandList->ClearRenderTargetView(renderTarget_->GetOffScreenHandle(RenderTargetType::EffectSystem_RenderTarget).handleCPU, clearColor, 0, nullptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::End() {
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), PreEffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectの作成をする
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::CreateEffect(const std::string& newName) {
	auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
	newEffect->Init(static_cast<EmitterShape>(createShape_));
	newEffect->SetEffectName(newName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Debug_Gui() {
	ImGui::RadioButton("Sphere", &createShape_, (int)EmitterShape::Sphere);
	ImGui::SameLine();
	ImGui::RadioButton("Cone", &createShape_, (int)EmitterShape::Cone);
	ImGui::SameLine();
	ImGui::RadioButton("Box", &createShape_, (int)EmitterShape::Box);

	if (ImGui::Button("CreateEffect")) {
		uint32_t effectNum = static_cast<uint32_t>(effectList_.size()) + 1;
		std::string defalutName = "effect" + std::to_string(effectNum);
		CreateEffect(defalutName);
	}

	//if (ImGui::BeginTabBar("EmitterTabs")) {
	//	// リストを反復してタブを作成
	//	for (auto it = effectList_.begin(); it != effectList_.end(); ++it) {
	//		// タブのラベルを生成（インデックスではなくポインタアドレスを利用）
	//		std::string tabLabel = (*it)->GetEmitterLabel() + std::to_string(std::distance(effectList_.begin(), it));

	//		if (ImGui::BeginTabItem(tabLabel.c_str())) {
	//			(*it)->Debug_Gui();

	//			ImGui::EndTabItem();
	//		}
	//	}
	//	ImGui::EndTabBar();
	//}

	static int selectedEffectIndex = -1; // -1 means no selection
	ImGui::Begin("Effect List");
	int index = 0;
	for (auto it = effectList_.begin(); it != effectList_.end(); ++it, ++index) {
		std::string name = (*it)->GetEmitterLabel();
		std::string label = "Effect_" + name + std::to_string(index);

		if (ImGui::Selectable(label.c_str(), selectedEffectIndex == index)) {
			selectedEffectIndex = index; // Update the selected index
		}
	}

	if (selectedEffectIndex >= 0 && selectedEffectIndex < effectList_.size()) {
		auto it = effectList_.begin();
		std::advance(it, selectedEffectIndex); // Move iterator to the selected index

		ImGui::Begin("EffectSystem");
		ImGui::Separator();
		(*it)->Debug_Gui();
		ImGui::End();
	}
	ImGui::End();
}
#endif // _DEBUG