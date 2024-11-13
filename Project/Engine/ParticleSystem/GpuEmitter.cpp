#include "GpuEmitter.h"

GpuEmitter::GpuEmitter() {
}

GpuEmitter::~GpuEmitter() {
}

void GpuEmitter::Init() {
	// gpuに送るResourceの作成
	emitterBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(SphereEmitter));

	emitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(SphereEmitter));
	emitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&sphereEmitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->frequency = 0.5f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 10;
	sphereEmitter_->emit = 0;
}

void GpuEmitter::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	sphereEmitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (sphereEmitter_->frequencyTime >= sphereEmitter_->frequency) {
		sphereEmitter_->frequencyTime -= sphereEmitter_->frequency;
		sphereEmitter_->emit = 1;
	} else {
		sphereEmitter_->emit = 0;
	}
}

void GpuEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, emitterBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, perFrameBuffer_->GetGPUVirtualAddress());
}

