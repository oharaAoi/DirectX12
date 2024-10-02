#pragma once
#include <memory>
#include "Enviroment.h"
#include "Engine/WinApp/WinApp.h"
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/DirectXCompiler/DirectXCompiler.h"
#include "Engine/DirectX/Pipeline/GraphicsPipelines.h"
#include "Engine/ComputeShader/ComputeShader.h"
#include "Engine/DirectX/Pipeline/PrimitivePipeline.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Input/Input.h"
#include "Engine/GameObject/Model.h"
#include "Engine/GameObject/Sphere.h"
#include "Engine/2d/Sprite.h"
#include "Engine/2d/Triangle.h"
#include "Engine/ParticleSystem/BaseParticle.h"
#include "Engine/Audio/Audio.h"
#include "Engine/Utilities/Shader.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/RenderTexture.h"
#include "Render.h"

enum class PipelineKind {
	kNormalPipeline,
	kTexturelessPipeline,
	kPBRPipeline,
	kParticlePipeline,
	kSpritePipeline,
	kSkinningPipeline
};

class EffectSystem;
class EffectSystemEditer;

class Engine {
public:

	Engine();
	~Engine();

	static void Initialize(uint32_t backBufferWidth, int32_t backBufferHeight);

	static void Finalize();

	static void DrawImGui();

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
	static void EndImGui();

	/// <summary>
	/// offScreenRenderingの処理を行う
	/// </summary>
	static void EndRenderTexture();


public:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 生成系
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 三角形のポインタを作成
	static std::unique_ptr<Triangle> CreateTriangle(const Mesh::Vertices& vertex);
	// スプライトのポインタを作成
	static std::unique_ptr<Sprite> CreateSprite(const Mesh::RectVetices& rect);
	static std::unique_ptr<Sprite> CreateSprite(const Vector2& centerPos, const Vector2& size);
	// 球のポインタを作成
	static std::unique_ptr<Sphere> CreateSphere(const uint32_t& devision);
	// モデルのポインタを作成
	static std::unique_ptr<Model> CreateModel(const std::string& directoryPath, const std::string& filePath);
	// パーティクルのポインタを作成
	static std::unique_ptr<BaseParticle> CreateBaseParticle(const std::string& directoryPath, const std::string& fileName, const uint32_t& instanceNum);
	// ワールドトランスフォームを作成
	static WorldTransform CreateWorldTransform();
	// Skinningの作成
	static std::unique_ptr<Skinning> CreateSkinning(Skeleton* skeleton, Model* model);


	/////////////////////////////////////////////////////////////////////////////////////////////
	// 描画系
	/////////////////////////////////////////////////////////////////////////////////////////////

	//static void DrawGameObject(BaseGameObject* gameObject, const GameObjectKind& kind);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 設定系
	/////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// パイプラインの設定
	/// </summary>
	/// <param name="kind">設定するパイプライン</param>
	static void SetPipeline(const PipelineKind& kind);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// sound系
	/////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Audioファイルを読み込む関数
	/// </summary>
	/// <param name="fileName">音声データのファイルパス</param>
	/// <returns></returns>
	static SoundData LoadAudio(const std::string& fileName);

	/// <summary>
	/// BGMを読み込む関数
	/// </summary>
	/// <param name="fileName">音声データのファイルパス</param>
	/// <returns></returns>
	static BgmData LoadBGM(const std::string& fileName);

	/// <summary>
	/// SEを読み込む関数
	/// </summary>
	/// <param name="fileName">音声データのファイルパス</param>
	/// <returns></returns>
	static SeData LoadSE(const std::string& fileName);

	/// <summary>
	/// 音を鳴らす関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	static void PlayAudio(const SoundData& soundData);

	/// <summary>
	/// BGMを鳴らす関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	/// <param name="isLoop">BGMをループさせるか</param>
	static void PlayBGM(const BgmData& soundData, const bool& isLoop);

	/// <summary>
	/// SEを鳴らす関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	/// <param name="isLoop">SEをループさせるか</param>
	static void PlaySE(const SeData& soundData, const bool& isLoop);

	/// <summary>
	/// BGMを一時停止させる関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	static void PauseBGM(const BgmData& soundData);

	/// <summary>
	/// BGMを再生する関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	static void ReStartBGM(const BgmData& soundData);

	/// <summary>
	/// 音の再生を止める関数
	/// </summary>
	/// <param name="soundData">音声データをまとめた構造体</param>
	static void StopBGM(const BgmData& soundData);

private:

};

// ======================================================== //
// 無名名前空間で内部リンゲージする
// ======================================================== //
namespace {
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	Render* render_ = nullptr;

	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	Input* input_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	// dxDevice
	std::shared_ptr<DirectXDevice> dxDevice_ = nullptr;
	// descriptorHeap
	std::shared_ptr<DescriptorHeap> descriptorHeap_ = nullptr;
	// dxCommand
	std::unique_ptr<DirectXCommands> dxCommands_ = nullptr;
	// renderTarget
	std::shared_ptr<RenderTarget> renderTarget_ = nullptr;
	// dxCompiler
	std::shared_ptr<DirectXCompiler> dxCompiler_ = nullptr;
	// pipeline
	std::unique_ptr<GraphicsPipelines> graphicsPipelines_ = nullptr;
	std::shared_ptr<PrimitivePipeline> primitivePipeline_ = nullptr;
	// CS
	std::unique_ptr<ComputeShader> computeShader_ = nullptr;
	// audio
	std::unique_ptr<Audio> audio_ = nullptr;
	// shaderファイルのパスをまとめたクラス
	std::shared_ptr<Shader> shaders_;

	// オフスクリーンレンダリングで生成したTextureを描画するクラス
	std::unique_ptr<RenderTexture> renderTexture_ = nullptr;

	static bool isRunCS_ = false;
}

