#include "GpuEffect.h"
#include "Engine/Engine.h"
#include "Engine/ParticleSystem/Emitter/SphereEmitter.h"
#include "Engine/ParticleSystem/Emitter/ConeEmitter.h"
#include "Engine/ParticleSystem/Emitter/BoxEmitter.h"

GpuEffect::GpuEffect() {}
GpuEffect::~GpuEffect() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Init(const EmitterShape& shape) {
	gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024);

	switch (shape) {
	case EmitterShape::Sphere:
		gpuEmitter_ = std::make_unique<SphereEmitter>();
		break;
	case EmitterShape::Cone:
		gpuEmitter_ = std::make_unique<ConeEmitter>();
		break;
	case EmitterShape::Box:
		gpuEmitter_ = std::make_unique<BoxEmitter>();
		break;
	default:
		break;
	}
	
	gpuEmitter_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Update() {
	gpuParticle_->SetPerViewProjectionMat(viewProjectionMat_);

	gpuEmitter_->Update();
	gpuParticle_->Update();

	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
	gpuParticle_->EmitBindCmdList(commandList, 0);
	gpuEmitter_->BindCmdList(commandList, 2);
	Dispatch(commandList, Vector3(1024 / 1024, 1, 1));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Draw() const {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	gpuParticle_->Draw(Engine::GetCommandList());

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	gpuEmitter_->DrawShape(viewProjectionMat_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CSの実行命令
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group) {
	commandList->Dispatch((UINT)group.x, (UINT)group.y, (UINT)group.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GpuEffect::Debug_Gui() {
	gpuEmitter_->Debug_Gui();
}
#endif