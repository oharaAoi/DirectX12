#pragma once
#include <memory>
#include "Enviroment.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DirectXDevice.h"
#include "DirectXCommands.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
#include "DirectXCompiler.h"
#include "GraphicsPipelines.h"
#include "ComputeShader.h"
#include "PrimitivePipeline.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Input.h"
// GameObject
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "BaseParticle.h"
// 2d
#include "PrimitiveDrawer.h"
// light
#include "LightGroup.h"
// audio
#include "Audio.h"
// data
#include "Shader.h"
// 
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "RenderTexture.h"

enum class PipelineKind {
	kNormalPipeline,
	kTexturelessPipeline,
	kPBRPipeline,
	kParticlePipeline,
	kSpritePipeline
};


enum class GameObjectKind {
	kModel,
	kSphere
};

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
	// 球のポインタを作成
	static std::unique_ptr<Sphere> CreateSphere(const uint32_t& devision);
	// モデルのポインタを作成
	static std::unique_ptr<Model> CreateModel(const std::string& filePath);
	// パーティクルのポインタを作成
	static std::unique_ptr<BaseParticle> CreateBaseParticle(const std::string& fileName, const uint32_t& instanceNum);
	// ワールドトランスフォームを作成
	static WorldTransform CreateWorldTransform();

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 描画系
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 三角形の描画
	static void DrawTriangle(Triangle* triangle, const WorldTransform& worldTransform);
	// スプライトの描画
	static void DrawSprite(Sprite* sprite);

	static void DrawSphere(Sphere* sphere, const WorldTransform& worldTransform);

	static void DrawModel(Model* model, const WorldTransform& worldTransform);

	static void DrawLine(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat);

	static void DrawParticle(BaseParticle* baseParticle, const uint32_t& numInstance);

	//static void DrawGameObject(BaseGameObject* gameObject, const GameObjectKind& kind);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 設定系
	/////////////////////////////////////////////////////////////////////////////////////////////
	static void SetLightKind(const LightGroup::LightKind& kind);

	static void SetEyePos(const Vector3& eyePos);

	/// <summary>
	/// パイプラインの設定
	/// </summary>
	/// <param name="kind">設定するパイプライン</param>
	static void SetPipeline(const PipelineKind& kind);

	/// <summary>
	/// 画面に移す範囲をここで設定する
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	static void SetViewProjection(const Matrix4x4& view, const Matrix4x4& projection);

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

	LightGroup::LightKind lightKind_;

	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	Input* input_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	// dxDevice
	std::unique_ptr<DirectXDevice> dxDevice_ = nullptr;
	// descriptorHeap
	std::unique_ptr<DescriptorHeap> descriptorHeap_ = nullptr;
	// dxCommand
	std::unique_ptr<DirectXCommands> dxCommands_ = nullptr;
	// renderTarget
	std::unique_ptr<RenderTarget> renderTarget_ = nullptr;
	// dxCompiler
	std::unique_ptr<DirectXCompiler> dxCompiler_ = nullptr;
	// pipeline
	std::unique_ptr<GraphicsPipelines> graphicesPipelines_ = nullptr;
	std::unique_ptr<PrimitivePipeline> primitivePipeline_ = nullptr;

	// CS
	std::unique_ptr<ComputeShader> grayScaleCS_ = nullptr;

	// light
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	// audio
	std::unique_ptr<Audio> audio_ = nullptr;
	// primitive
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_ = nullptr;
	// shaderファイルのパスをまとめたクラス
	std::unique_ptr<Shader> shaders_;

	// viewProjection
	std::unique_ptr<ViewProjection> viewProjection_ = nullptr;
	// オフスクリーンレンダリングで生成したTextureを描画するクラス
	std::unique_ptr<RenderTexture> renderTexture_ = nullptr;
}

