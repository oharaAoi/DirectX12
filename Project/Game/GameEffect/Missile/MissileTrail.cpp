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
	gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024);

	gpuEmitter_ = std::make_unique<MissileTrailEmitter>();
	gpuEmitter_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void MissileTrail::Update() {
	gpuParticle_->Update();

	if (!gpuEmitter_->GetIsDead()) {
		gpuEmitter_->Update();

		ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
		Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
		gpuParticle_->EmitBindCmdList(commandList, 0);
		gpuEmitter_->BindCmdList(commandList, 2);

		Dispatch(commandList, Vector3(1024 / 1024, 1, 1));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissileTrail::Draw() const {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	gpuParticle_->Draw(Engine::GetCommandList());
}
