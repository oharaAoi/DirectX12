#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DirectXDevice.h"
#include "DirectXCommands.h"
#include "DescriptorHeap.h"
#include "Pipeline.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
// GameObject
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
// light
#include "LightGroup.h"

class Engine {
public:

	Engine();
	~Engine();

	static void Initialize(uint32_t backBufferWidth, int32_t backBufferHeight);

	static void Finalize();

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

	static std::unique_ptr<Sprite> CreateSprite(const Mesh::RectVetices& rect);

	static std::unique_ptr<Sphere> CreateSphere(const uint32_t& devision);

	static std::unique_ptr<Model> CreateModel(const std::string& filePath);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 描画系
	/////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawTriangle(Triangle* triangle);

	static void DrawSprite(Sprite* sprite);

	static void DrawSphere(Sphere* sphere);

	static void DrawModel(Model* model);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 設定系
	/////////////////////////////////////////////////////////////////////////////////////////////
	static void SetLightKind(const LightGroup::LightKind& kind);

private:

};

// ======================================================== //
// 無名名前空間で内部リンゲージする
// ======================================================== //
namespace {
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	LightGroup::LightKind lightKind_;

	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	std::unique_ptr<DirectXDevice> dxDevice_ = nullptr;

	std::unique_ptr<DescriptorHeap> descriptorHeap_ = nullptr;

	std::unique_ptr<DirectXCommands> dxCommands_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;
	std::unique_ptr<Pipeline> texturelessPipeline_ = nullptr;

	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
}

