#include "GameTimer.h"

float GameTimer::kDeletaTime_ = 0.0f;

GameTimer::GameTimer(const uint32_t& fps) {
	frameDuration_ = std::chrono::milliseconds(1000 / fps);
	preFrameTime_ = std::chrono::steady_clock::now();
	kDeletaTime_ = 1.0f / static_cast<float>(fps);
}

GameTimer::~GameTimer() {
}

void GameTimer::WaitNextFrame() {
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - preFrameTime_;

	if (elapsedTime < frameDuration_) {
		std::this_thread::sleep_for(frameDuration_ - elapsedTime);
	}

	preFrameTime_ = std::chrono::steady_clock::now();
}
