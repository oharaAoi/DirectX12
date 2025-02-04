#pragma once
#include <unordered_map>
#include <memory>

#include "Game/Effect/BaseCpuParticle.h"
#include "Game/Effect/HitSparkEffect.h"
#include "Game/Effect/SwordEffect.h"
#include "Game/Effect/EnemyDownEffect.h"

enum class ParticlesType {
	HitSpark,
	SwordEffect,
	EnemyDown
};

class ParticleManager {
public:

	ParticleManager() = default;
	~ParticleManager() {};

	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

	static ParticleManager* GetInstance();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;

	void CreateParticle(const ParticlesType& type, const Vector3& pos, uint32_t createNum);

private:

	std::unique_ptr<HitSparkEffect> hitSparkEffect_;
	std::unique_ptr<SwordEffect> swordEffect;
	std::unique_ptr<EnemyDownEffect> enemyDown_;

};

