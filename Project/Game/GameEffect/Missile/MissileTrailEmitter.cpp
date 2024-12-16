#include "MissileTrailEmitter.h"

MissileTrailEmitter::MissileTrailEmitter() {
}

MissileTrailEmitter::~MissileTrailEmitter() {
}

void MissileTrailEmitter::Init() {
	// gpuに送るResourceの作成
	emitterBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(SphereEmitter));

	emitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(SphereEmitter));
	emitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	emitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	emitter_->translate = Vector3::ZERO();
	emitter_->frequency = 0.5f;
	emitter_->frequencyTime = 0.0f;
	emitter_->count = 5;
	emitter_->emit = 0;
	emitter_->speed = 1.0f;
	emitter_->radius = 1.0f;
	emitter_->emissionType = 1;
	emitter_->color = Vector4(1, 0, 0, 1);
}

void MissileTrailEmitter::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	emitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (emitter_->frequencyTime >= emitter_->frequency) {
		emitter_->frequencyTime -= emitter_->frequency;
		emitter_->emit = 1;
	} else {
		emitter_->emit = 0;
	}
}

void MissileTrailEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, emitterBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, perFrameBuffer_->GetGPUVirtualAddress());
}
