#include "Engine.h"

Engine::Engine() {
}

Engine::~Engine() {
	
}

void Engine::Initialize(uint32_t backBufferWidth, int32_t backBufferHeight) {
	kClientWidth_ = backBufferWidth;
	kClientHeight_ = backBufferHeight;

	// ↓インスタンスの生成
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstacne();

	// ↓各初期化
	winApp_->CreateGameWindow();
	dxCommon_->Initialize(winApp_, kClientWidth_, kClientHeight_);
	dxDevice_ = std::make_unique<DirectXDevice>(dxCommon_->GetUseAdapter());

	assert(dxDevice_->GetDevice());

	dxCommands_ = std::make_unique<DirectXCommands>(dxDevice_->GetDevice());
	descriptorHeap_ = std::make_unique<DescriptorHeap>(dxDevice_->GetDevice());

	dxCommon_->Setting(dxDevice_->GetDevice(), dxCommands_.get(), descriptorHeap_.get());

	pipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice());

	Log("Clear!\n");
}

void Engine::Finalize() {
	pipeline_->Finalize();
	descriptorHeap_->Finalize();
	dxCommands_->Finalize();
	dxCommon_->Finalize();
	dxDevice_->Finalize();
	winApp_->Finalize();
}

bool Engine::ProcessMessage() {
	return  winApp_->ProcessMessage();
}

void Engine::BeginFrame() {
	dxCommon_->Begin();
}

void Engine::EndFrame() {
	dxCommon_->End();
}

//------------------------------------------------------------------------------------------------------
// 生成
//------------------------------------------------------------------------------------------------------
std::unique_ptr<Triangle> Engine::CreateTriangle(const Mesh::Vertices& vertex) {
	std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>();
	triangle->Init(dxDevice_->GetDevice(), vertex);
	return triangle;
}

//------------------------------------------------------------------------------------------------------
// 描画
//------------------------------------------------------------------------------------------------------
void Engine::DrawTriangle(Triangle* triangle) {
	pipeline_->Draw(dxCommands_->GetCommandList());
	triangle->Draw(dxCommands_->GetCommandList());
}