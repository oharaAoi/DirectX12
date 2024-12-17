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
	virtual void Update() = 0;
	virtual void Draw() const = 0;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

private:

	WorldTransform* parentWorldTransform_ = nullptr;

};

