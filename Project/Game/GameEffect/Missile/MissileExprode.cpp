#include "MissileExprode.h"

MissileExprode::MissileExprode() {
}

MissileExprode::~MissileExprode() {
}

void MissileExprode::Init() {
	gpuEmitter_ = std::make_unique<MissileExprodeEmitter>();
	gpuEmitter_->Init();
	gpuEmitter_->SetEmitterPos(emitPos_);

	isDead_ = false;
}

void MissileExprode::Update(GpuParticle* gpuParticle) {
	if (gpuEmitter_->GetIsDead()) {
		isDead_ = true;
		return;
	}

	if (parentWorldTransform_ != nullptr) {
		gpuEmitter_->SetEmitterPos(Transform(Vector3::ZERO(), parentWorldTransform_->GetWorldMatrix()));
	}

	if (!gpuEmitter_->GetIsDead()) {
		gpuEmitter_->Update();

		ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
		Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
		gpuParticle->EmitBindCmdList(commandList, 0);
		gpuEmitter_->BindCmdList(commandList, 2);

		Dispatch(commandList, Vector3(1024 / 1024, 1, 1));
	}
}
