#pragma once
#include "Engine.h"
#include "Particle.h"
#include "MathStructures.h"
#include "BoxCollider.h"
#include "DrawUtils.h"

class ParticleField {
public: // データ構造体

	/// <summary>
	/// 加速度を与えるField
	/// </summary>
	struct AccelerationField {
		Vector3 acceleration; // 加速度
		AABB area;			  // 範囲
	};

public:

	ParticleField();
	~ParticleField();

	void Init();

	void Update();

	void Draw(const Matrix4x4& vpMatrix);

public:

	/// <summary>
	/// 当たり判定を取るParticleをセットする
	/// </summary>
	/// <param name="particle"></param>
	void SetParticle(Particle* particle) {
		particle_ = particle;
	}

private:

	AccelerationField accelerationField_;

	Particle* particle_ = nullptr;

	bool onField_;

};

