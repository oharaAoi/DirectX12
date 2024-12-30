#pragma once
#include <array>
#include "Render.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Lib/GameTimer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class BaseEffect;

class Emitter {
public:

	Emitter(BaseEffect* effect);
	~Emitter();


	void Init();
	void Update();
	void Draw(const Matrix4x4& viewProjection);

	// パーティクルの生成
	void CreateParticle();

	void Load(const std::string& fileName);

#ifdef _DEBUG
	void Debug_Gui();

	void Save();

private:

	char exportFileName_[64];
	char loadFileName_[64];

#endif

public:

	const bool GetIsDead() const { return isDead_; }

private:

	const std::string kDirectoryPath_ = "./Game/Resources/GameData/Emitter/";

	bool isRangeDraw_;

	// 保存するデータ
	kTransform transform_;	// 場所
	uint32_t count_;		// 発生個数
	float frequency_;		// 発生頻度
	Vector3 size_;			// エミッターのサイズ
	Vector3 firstVelocity_;	// 初速度
	Vector3 acceleration_;	// particleの加速度
	float lifeTime_;		// 生存期間
	float emitterLifeTime_; // Emitterの生存期間

	// emitter内部で使用する変数
	float frequencyTime_;	// 頻度用の時刻
	bool isDead_;			// Emitterが機能しているか

	// effectのポインタ
	BaseEffect* effect_;
};

