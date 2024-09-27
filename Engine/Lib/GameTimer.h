#pragma once
#include <cstdint>
#include <chrono>
#include <thread>

/// <summary>
/// フレームレートを固定するためのクラス
/// </summary>
class GameTimer {
public:

	GameTimer(const uint32_t& fps);
	~GameTimer();

	void WaitNextFrame();

	static float DeltaTime() { return kDeletaTime_; }

private:
	std::chrono::steady_clock::time_point preFrameTime_;	// 前フレームの時間
	std::chrono::milliseconds frameDuration_;				// フレームの間隔
	static float kDeletaTime_;
};

