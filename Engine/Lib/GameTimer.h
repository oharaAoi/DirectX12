#pragma once
#include <cstdint>
#include <chrono>
#include <thread>

// 1/60ピッタリの時間
const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
// 1/60よりわずかに短い時間
const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 6.0f));

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
	static float kDeletaTime_;
};

