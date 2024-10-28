#include "EffectSystemEditer.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"

EffectSystemEditer::EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	Init(renderTarget, descriptorHeaps, dxCommands, device);
}

EffectSystemEditer::~EffectSystemEditer() {}

void EffectSystemEditer::Finalize() {
	depthStencilResource_.Reset();
	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin();
			 effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Finalize();

			++effectListIter;
		}
		++effectDataListIter;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
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

	CreateEffect();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Update() {
	effectSystemCamera_->Update();

	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<Emitter>>::iterator emitterListIter = effectDataListIter->emitterList.begin(); emitterListIter != effectDataListIter->emitterList.end();) {
			(*emitterListIter)->Update();
			ImGui::Begin("Editor");
			(*emitterListIter)->Debug_Gui();
			ImGui::End();
			++emitterListIter;
		}

		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin(); effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->SetCameraMatrix(effectSystemCamera_->GetCameraMatrix());
			(*effectListIter)->Update(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());
			ImGui::Begin("Editor");
			(*effectListIter)->Debug_Gui();
			ImGui::End();
			++effectListIter;
		}

		++effectDataListIter;
	}

	//DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Draw() const {
	for (std::list<EffectData>::const_iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::const_iterator effectListIter = effectDataListIter->effectList.begin();
			 effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Draw();
			++effectListIter;
		}

		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		/*for (std::list<std::unique_ptr<Emitter>>::const_iterator emitterListIter = effectDataListIter->emitterList.begin();
			 emitterListIter != effectDataListIter->emitterList.end();) {
			(*emitterListIter)->Draw(effectSystemCamera_->GetViewMatrix() * effectSystemCamera_->GetProjectionMatrix());
			++emitterListIter;
		}*/

		++effectDataListIter;
	}

	// 最後にImGui上でEffectを描画する
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ImGui::Begin("Render Target View");
	ImTextureID textureID = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetOffScreenSRVHandle(RenderTargetType::EffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::Image((void*)textureID, ImVec2(static_cast<float>(640), static_cast<float>(360))); // サイズは適宜調整
	ImGui::End();
}

void EffectSystemEditer::CreateEffect() {
	EffectData effectData;
	std::unique_ptr<BaseEffect> effect = std::make_unique<BaseEffect>();
	effect->Init("./Engine/Resources/Effect/", "particle.obj", 100);
	std::unique_ptr<Emitter> emitter = std::make_unique<Emitter>(effect.get());

	effectData.emitterList.push_back(std::move(emitter));
	effectData.effectList.push_back(std::move(effect));

	effectList_.push_back(std::move(effectData));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
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

void EffectSystemEditer::End() {
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}
#endif // _DEBUG