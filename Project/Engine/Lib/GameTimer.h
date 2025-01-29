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

	void Reset();
	void CalculationFrame();

	void Debug();

	static float DeltaTime();
	static float TotalTime(){ return totalTime_; }
	static float TimeRate() { return timeRate_; }

	static void SetIsStop(bool isStop) { isStop_ = isStop; }

	static float fps_;

private:
	std::chrono::steady_clock::time_point preFrameTime_;	// 前フレームの時間
	std::chrono::milliseconds frameDuration_;				// フレームの間隔
	static float deletaTime_;
	static float timeRate_;
	static float totalTime_;

	static bool isStop_;

	uint16_t stopCount_;
};

const float kDeltaTime_ = (1.0f / GameTimer::fps_);