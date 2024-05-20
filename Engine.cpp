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

	// ↓各初期化
	winApp_->CreateGameWindow();
}

bool Engine::ProcessMessage() {
	return  winApp_->ProcessMessage();
}
