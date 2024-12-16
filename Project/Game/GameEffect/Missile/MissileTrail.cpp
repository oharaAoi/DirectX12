#include "MissileTrail.h"
#include "Engine/Engine.h"

MissileTrail::MissileTrail() {
}

MissileTrail::~MissileTrail() {
}

void MissileTrail::Init() {
	gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024);

	gpuEmitter_ = std::make_unique<MissileTrailEmitter>();
	gpuEmitter_->Init();
}

void MissileTrail::Update() {
	gpuEmitter_->Update();
	gpuParticle_->Update();

	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
	gpuParticle_->EmitBindCmdList(commandList, 0);
	gpuEmitter_->BindCmdList(commandList, 2);
	Dispatch(commandList, Vector3(1024 / 1024, 1, 1));
}

void MissileTrail::Draw() const {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	gpuParticle_->Draw(Engine::GetCommandList());
}

void MissileTrail::Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group) {
	commandList->Dispatch((UINT)group.x, (UINT)group.y, (UINT)group.z);
}
