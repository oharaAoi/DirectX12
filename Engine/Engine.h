#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DirectXDevice.h"
#include "DirectXCommands.h"
#include "DescriptorHeap.h"

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

	static void BeginFrame();

	static void EndFrame();

private:

};

// ======================================================== //
// 無名名前空間で内部リンゲージする
// ======================================================== //
namespace {
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	std::unique_ptr<DirectXDevice> dxDevice_ = nullptr;

	std::unique_ptr<DescriptorHeap> descriptorHeap_ = nullptr;

	std::unique_ptr<DirectXCommands> dxCommands_ = nullptr;
}

