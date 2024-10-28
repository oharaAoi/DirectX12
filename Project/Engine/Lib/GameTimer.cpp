#include "GameTimer.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

float GameTimer::deletaTime_ = 0.0f;
float GameTimer::fps_ = 60.0f;
float GameTimer::timeRate_ = 0.0f;
float GameTimer::totalTime_ = 0.0f;

GameTimer::GameTimer(const uint32_t& fps) {
	frameDuration_ = std::chrono::milliseconds(1000 / fps);
	preFrameTime_ = std::chrono::steady_clock::now();
	deletaTime_ = 1.0f / static_cast<float>(fps);
}

GameTimer::~GameTimer() {
}

void GameTimer::CalculationFrame() {
	auto currentTime = std::chrono::steady_clock::now();
	
	deletaTime_ = std::chrono::duration<float>(currentTime - preFrameTime_).count();
	timeRate_ = deletaTime_ / kDeltaTime_;
	totalTime_ += deletaTime_;
	preFrameTime_ = currentTime;
}

void GameTimer::FPS() {
#ifdef _DEBUG
	ImGui::Begin("Timer");
	float fps = 1.0f / deletaTime_;
	ImGui::Text("fps: %f", fps);

	ImGui::Text("1f: %f", deletaTime_ * 1000.0f);
	ImGui::End();
#endif
}
