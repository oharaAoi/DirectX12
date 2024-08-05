#pragma once
#include <array>
#include "Render.h"
#include "Vector3.h"

class BaseEffect;

class Emitter {
public:

	Emitter(BaseEffect* effect);
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

private:

	bool isRangeDraw_;

	kTransform transform_;	// 場所
	uint32_t count_;			// 発生個数
	float frequency_;		// 発生頻度
	float frequencyTime_;	// 頻度用の時刻
	Vector3 size_;			// エミッターのサイズ
	Vector3 firstVelocity_;	// 初速度
	float lifeTime_;			// 生存期間

	// effectのポインタ
	BaseEffect* effect_;
};

