#pragma once
#include "WinApp.h"

class Engine {
public:

	Engine();
	~Engine();

	static void Initialize(uint32_t backBufferWidth, int32_t backBufferHeight);

public:

	/// <summary>
	/// ループの判定
	/// </summary>
	/// <returns></returns>
	static bool ProcessMessage();

private:

};

// ======================================================== //
// 無名名前空間で内部リンゲージする
// ======================================================== //
namespace {
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	WinApp* winApp_ = nullptr;
}

