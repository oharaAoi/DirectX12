#pragma once
#include "Game/Effect/BaseCpuParticle.h"

class EnemyDownEffect :
	public BaseCpuParticle {
public:

	EnemyDownEffect() {};
	~EnemyDownEffect() override {};

	void Init();
	void Update();

	void MakeParticle(const Vector3& pos, uint32_t createNum) override;

private:
};

