#include "MissileTrail.h"
#include "Engine/Engine.h"

MissileTrail::MissileTrail() {
}

MissileTrail::~MissileTrail() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissileTrail::Init() {
	gpuEmitter_ = std::make_unique<MissileTrailEmitter>();
	gpuEmitter_->Init();
	gpuEmitter_->SetEmitterPos(emitPos_);

	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void MissileTrail::Update(GpuParticle* gpuParticle) {
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
