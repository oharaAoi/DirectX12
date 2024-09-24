#include "EffectSystemEditer.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"

EffectSystemEditer::EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	Init(renderTarget, descriptorHeaps, dxCommands, device);
}

EffectSystemEditer::~EffectSystemEditer() {
}

void EffectSystemEditer::Finalize() {
	depthStencilResource_.Reset();
}

void EffectSystemEditer::Init(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {

	renderTarget_ = renderTarget;
	descriptorHeaps_ = descriptorHeaps;
	dxCommands_ = dxCommands;
	device_ = device;

	

	depthStencilResource_ = CreateDepthStencilTextureResource(device_, kClientWidth_, kClientHeight_);

	// DSVの生成
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device_->CreateDepthStencilView(depthStencilResource_.Get(), &desc, descriptorHeaps_->GetDescriptorHandle(DescriptorHeapType::TYPE_DSV).handleCPU);

	// Cameraの初期化
	effectSystemCamera_ = std::make_unique<EffectSystemCamera>();
}

void EffectSystemEditer::Update() {
	effectSystemCamera_->Update();
}

void EffectSystemEditer::Draw() const {

	DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());

	// 最後にImGui上でEffectを描画する
	renderTarget_->ChangeRTVResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ImGui::Begin("Render Target View");
	ImTextureID textureID = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetOffScreenSRVHandle(RenderTargetType::EffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::Image((void*)textureID, ImVec2(static_cast<float>(640), static_cast<float>(360))); // サイズは適宜調整
	ImGui::End();
}

void EffectSystemEditer::Begin() {
	// ここでゲーム描画のRenderTargetからEffect用のRenderTargetに変更する
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	// dsvのポインターを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle.ptr += size_t(descriptorHeaps_->GetDescriptorSize()->GetDSV());
	// RenderTargetを指定する
	renderTarget_->OMSetRenderTarget(commandList, RenderTargetType::EffectSystem_RenderTarget, dsvHandle);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	// RenderTargetをクリアする
	commandList->ClearRenderTargetView(renderTarget_->GetOffScreenHandle(RenderTargetType::EffectSystem_RenderTarget).handleCPU, clearColor, 0, nullptr);
}

void EffectSystemEditer::End() {
	renderTarget_->ChangeRTVResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}
#endif // _DEBUG