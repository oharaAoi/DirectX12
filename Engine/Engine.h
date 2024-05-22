#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DirectXDevice.h"
#include "DirectXCommands.h"
#include "DescriptorHeap.h"
#include "Pipeline.h"
// GameObject
#include "Triangle.h"

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

	/// <summary>
	/// フレームを開始する
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// フレームを終了する
	/// </summary>
	static void EndFrame();

public:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 生成系
	/////////////////////////////////////////////////////////////////////////////////////////////
	static std::unique_ptr<Triangle> CreateTriangle(const Mesh::Vertices& vertex);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 描画系
	/////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawTriangle(Triangle* triangle);

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

	std::unique_ptr<Pipeline> pipeline_ = nullptr;
}

