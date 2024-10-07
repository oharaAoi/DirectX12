#include "GameTimer.h"

float GameTimer::kDeletaTime_ = 0.0f;

GameTimer::GameTimer(const uint32_t& fps) {
	preFrameTime_ = std::chrono::steady_clock::now();
	kDeletaTime_ = 1.0f / static_cast<float>(fps);
}

GameTimer::~GameTimer() {
}

void GameTimer::WaitNextFrame() {
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	std::chrono::microseconds elapsedTime = 
		std::chrono::duration_cast<std::chrono::microseconds>(currentTime - preFrameTime_);

	// 1/60たっていない場合
	if (elapsedTime < kMinTime) {
		while (std::chrono::steady_clock::now() - preFrameTime_ < kMinTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	preFrameTime_ = std::chrono::steady_clock::now();
}
