#include "GameTimer.h"
#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
#endif

float GameTimer::deletaTime_ = 0.0f;
float GameTimer::fps_ = 60.0f;
float GameTimer::timeRate_ = 0.0f;
float GameTimer::totalTime_ = 0.0f;
bool GameTimer::isStop_ = false;

GameTimer::GameTimer(const uint32_t& fps) {
	frameDuration_ = std::chrono::milliseconds(1000 / fps);
	preFrameTime_ = std::chrono::steady_clock::now();
	deletaTime_ = 1.0f / static_cast<float>(fps);
}

GameTimer::~GameTimer() {
}

void GameTimer::Reset() {
	if (isStop_) {
		stopCount_++;
	}
}

void GameTimer::CalculationFrame() {
	auto currentTime = std::chrono::steady_clock::now();
	
	deletaTime_ = std::chrono::duration<float>(currentTime - preFrameTime_).count();
	timeRate_ = deletaTime_ / kDeltaTime_;
	totalTime_ += deletaTime_;
	preFrameTime_ = currentTime;
}

void GameTimer::Debug() {
#ifdef _DEBUG
	ImGui::Begin("GameTimer", nullptr);
	float fps = 1.0f / deletaTime_;
	ImGui::Text("%f fps", fps);
	ImGui::Text("%f m/s", deletaTime_ * 1000.0f);
	ImGui::End();
#endif
}

float GameTimer::DeltaTime() {
	if (isStop_) {
		return 0.0f;
	} else {
		return deletaTime_;
	}
}
