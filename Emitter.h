#pragma once
#include <array>
#include "Engine.h"
#include "Vector3.h"

class Particle;

class Emitter {
public: // データ構造体

	struct EmitterData {
		kTransform transform;	// 場所
		uint32_t count;			// 発生個数
		float frequency;		// 発生頻度
		float frequencyTime;	// 頻度用の時刻
		Vector3 size;			// エミッターのサイズ
		Vector3 firstVelocity;	// 初速度
		float lifeTime;			// 生存期間
	};

public:

	Emitter();
	~Emitter();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	///　描画
	/// </summary>
	void Draw(const Matrix4x4& viewProjection);

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw();

public:

	// パーティクルの生成
	void CreateParticle();

	/// <summary>
	/// パーティクルの設定
	/// </summary>
	/// <param name="particle"></param>
	void SetParticle(Particle* particle) {
		particle_ = particle;
	}

private:

	EmitterData emitterData_;

	bool isRangeDraw_;

	// 
	Particle* particle_ = nullptr;
};

