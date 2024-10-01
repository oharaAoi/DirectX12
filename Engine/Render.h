#pragma once
#include "Engine/2d/Triangle.h"
#include "Engine/2d/Sprite.h"
#include "Engine/GameObject/Sphere.h"
#include "Engine/GameObject/Model.h"
#include "Engine/ParticleSystem/BaseParticle.h"
#include "Engine/DirectX/Pipeline/PrimitivePipeline.h"
#include "Engine/2d/PrimitiveDrawer.h"
#include "Engine/Light/LightGroup.h"

/// <summary>
/// GameObjectの描画を行う
/// </summary>
class Render {
public:

	Render();
	~Render();
	Render(const Render&) = delete;
	const Render& operator=(const Render&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Render* GetInstacne();

	void Init(ID3D12GraphicsCommandList* commandList, ID3D12Device* device, PrimitivePipeline* primitive);

	void Finalize();

	static void Update();

public:

	/// <summary>
	/// 三角形の描画
	/// </summary>
	/// <param name="triangle">: 三角形のポインタ</param>
	/// <param name="worldTransform">: worldTrnasform</param>
	static void DrawTriangle(Triangle* triangle, const WorldTransform& worldTransform);
	
	/// <summary>
	/// Spriteの描画
	/// </summary>
	/// <param name="sprite">: spriteのポインタ</param>
	static void DrawSprite(Sprite* sprite);

	/// <summary>
	/// 球体の描画
	/// </summary>
	/// <param name="sphere">: sphereのポインタ</param>
	/// <param name="worldTransform">: worldTransform</param>
	static void DrawSphere(Sphere* sphere, const WorldTransform& worldTransform);

	/// <summary>
	/// モデルの描画
	/// </summary>
	/// <param name="model">: モデルのポインタ</param>
	/// <param name="worldTransform">: worldTransform</param>
	static void DrawModel(Model* model, const WorldTransform& worldTransform);

	static void DrawAnimationModel(Model* model, const Skinning& skeleton, const WorldTransform& worldTransform);

	/// <summary>
	/// Particleの描画
	/// </summary>
	/// <param name="baseParticle">: particleの描画</param>
	/// <param name="numInstance">: 何個描画するか</param>
	static void DrawParticle(BaseParticle* baseParticle, const uint32_t& numInstance);

	/// <summary>
	/// 線の描画
	/// </summary>
	/// <param name="p1">: 始点</param>
	/// <param name="p2">: 終点</param>
	/// <param name="color">: 色</param>
	/// <param name="vpMat">: viewProjection</param>
	static void DrawLine(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat);

	//==================================================================================
	// ↓　設定系
	//==================================================================================

	/// <summary>
	/// 画面に移す範囲をここで設定する
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	static void SetViewProjection(const Matrix4x4& view, const Matrix4x4& projection);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	static void SetViewProjection2D(const Matrix4x4& view, const Matrix4x4& projection);

	static void SetEyePos(const Vector3& eyePos);

private:


};

namespace {
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection2D_ = nullptr;

	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_ = nullptr;
	PrimitivePipeline* primitivePipelines_ = nullptr;
}
