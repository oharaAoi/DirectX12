#pragma once
#include "Game/Effect/BaseCpuParticle.h"

class SwordEffect :
	public BaseCpuParticle {
public:

	SwordEffect() {};
	~SwordEffect() override {};

	void Init();
	void Update();

	void MakeParticle(const Vector3& pos, uint32_t createNum) override;

private:

	float speed_ = 8.0f;
	float lifeTime_ = 1.0f;

};

