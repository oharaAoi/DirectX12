#include "MissileExprodeEmitter.h"

MissileExprodeEmitter::MissileExprodeEmitter() {
}

MissileExprodeEmitter::~MissileExprodeEmitter() {
}

void MissileExprodeEmitter::Init() {
	GpuEmitter::Init();

	sphereEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sphereEmitter_->scale = Vector3(0.4f, 0.4f, 0.4f);
	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->frequency = 0.2f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 8;
	sphereEmitter_->emit = 0;
	sphereEmitter_->speed = 8.0f;
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->emissionType = 1;
	sphereEmitter_->lifeTime = 1.0f;
	sphereEmitter_->color = Vector4(1, 0, 0, 1);

	emitterParameter_.lifeTime = 0.5f;

	isDead_ = false;
}

void MissileExprodeEmitter::Update() {
	GpuEmitter::Update();
	emitterParameter_.lifeTime -= GameTimer::DeltaTime();
	if (emitterParameter_.lifeTime <= 0.0f) {
		isDead_ = true;
	}
}

void MissileExprodeEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
}
