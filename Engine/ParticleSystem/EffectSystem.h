#pragma once
#include "Emitter.h"
#include "ParticleField.h"
// Effect
#include "BaseEffect.h"
#include "EmissionEffect.h"

class EffectSystem {
public:

	/// <summary>
	/// Effectのデータ(EmitterとEffect)
	/// </summary>
	struct EffectData {
		std::list<std::unique_ptr<Emitter>> emitterList;
		std::list<std::unique_ptr<BaseEffect>> effectList;
	};
	
public:

	EffectSystem() = default;
	~EffectSystem();
	EffectSystem(const EffectSystem&) = delete;
	const EffectSystem& operator=(const EffectSystem&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static EffectSystem* GetInstacne();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// Effectを生成する
	/// </summary>
	void CreateEffect();

public: // accessor

	void SetCameraMatrix(const Matrix4x4& cameraMat) { cameraMat_ = cameraMat; }
	void SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection);

private:

	std::unique_ptr<ParticleField> particleField_ = nullptr;
	std::list<EffectData> effectList_;

	Matrix4x4 cameraMat_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

};

