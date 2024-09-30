#pragma once
#include <array>
#include "Render.h"
#include "Engine/Math/Vector3.h"

class BaseEffect;

class Emitter {
public:

	Emitter(BaseEffect* effect);
	~Emitter();


	void Init();
	void Update();
	void Draw(const Matrix4x4& viewProjection);

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	// パーティクルの生成
	void CreateParticle();

private:

	bool isRangeDraw_;

	kTransform transform_;	// 場所
	uint32_t count_;		// 発生個数
	float frequency_;		// 発生頻度
	float frequencyTime_;	// 頻度用の時刻
	Vector3 size_;			// エミッターのサイズ
	Vector3 firstVelocity_;	// 初速度
	float lifeTime_;		// 生存期間

	// effectのポインタ
	BaseEffect* effect_;
};

