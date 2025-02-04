#pragma once
#include "Game/Effect/BaseCpuParticle.h"

class HitSparkEffect :
	public BaseCpuParticle {
public:

	HitSparkEffect() {};
	~HitSparkEffect() override {};

	void Init();
	void Update();

	void MakeParticle(const Vector3& pos, uint32_t createNum) override;

private:

	float speed_ = 8.0f;

};

