#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/ParticleSystem/GpuParticle.h"
#include "Engine/ParticleSystem/GpuEmitter.h"
#include "Engine/Assets/WorldTransform.h"

class IGameEffect {
public:

	virtual ~IGameEffect() = default;

	virtual void Init() = 0;
	virtual void Update(GpuParticle* gpuParticle) = 0;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

	void SetEmitPos(const Vector3& pos) { emitPos_ = pos; }

	void SetParentWorldTransform(WorldTransform* transform) { parentWorldTransform_ = transform; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }
	const bool GetIsDead() const { return isDead_; }

protected:

	WorldTransform* parentWorldTransform_ = nullptr;
	Vector3 emitPos_;

	bool isDead_;
};

