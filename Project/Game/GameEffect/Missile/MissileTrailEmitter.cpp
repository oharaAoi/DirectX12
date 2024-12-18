#include "MissileTrailEmitter.h"

MissileTrailEmitter::MissileTrailEmitter() {
}

MissileTrailEmitter::~MissileTrailEmitter() {
}

void MissileTrailEmitter::Init() {
	GpuEmitter::Init();

	sphereEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sphereEmitter_->scale = Vector3(0.4f, 0.4f, 0.4f);
	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->frequency = 0.2f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 6;
	sphereEmitter_->emit = 0;
	sphereEmitter_->speed = 0.0f;
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->emissionType = 1;
	sphereEmitter_->lifeTime = 1;
	sphereEmitter_->color = Vector4(1, 1, 1, 1);
}

void MissileTrailEmitter::Update() {
	GpuEmitter::Update();
}

void MissileTrailEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
}
