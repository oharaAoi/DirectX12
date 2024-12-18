#include "MissileHitExprodeEmitter.h"

MissileHitExprodeEmitter::MissileHitExprodeEmitter() {
}

MissileHitExprodeEmitter::~MissileHitExprodeEmitter() {
}

void MissileHitExprodeEmitter::Init() {
	GpuEmitter::Init();

	sphereEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sphereEmitter_->scale = Vector3(0.5f, 0.5f, 0.5f);
	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->frequency = 0.2f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 20;
	sphereEmitter_->emit = 0;
	sphereEmitter_->speed = 6.0f;
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->emissionType = 0;
	sphereEmitter_->lifeTime = 1;
	sphereEmitter_->color = Vector4(1, 0, 0, 1);

	emitterParameter_.lifeTime = 0.5f;

	isDead_ = false;

}

void MissileHitExprodeEmitter::Update() {
	GpuEmitter::Update();
	emitterParameter_.lifeTime -= GameTimer::DeltaTime();
	if (emitterParameter_.lifeTime <= 0.0f) {
		isDead_ = true;
	}
}

void MissileHitExprodeEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
}
