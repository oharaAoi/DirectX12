#include "GpuEffect.h"
#include "Engine/Engine.h"
#include "Engine/System/ParticleSystem/Emitter/SphereEmitter.h"
#include "Engine/System/ParticleSystem/Emitter/ConeEmitter.h"
#include "Engine/System/ParticleSystem/Emitter/BoxEmitter.h"

GpuEffect::GpuEffect() {}
GpuEffect::~GpuEffect() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Init(const EmitterShape& shape) {
	/*gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024);*/

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

	//// ポインタの取得
	//DescriptorHeap* dxHeap = Engine::GetDxHeap();
	//ID3D12Device* dxDevice = Engine::GetDevice();

	
	isAlive_ = true;
	allAlphaZero_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Update() {
	/*if (gpuEmitter_->GetIsDead()) {
		isAlive_ = false;
		return;
	}*/
	//gpuParticle_->SetPerViewProjectionMat(viewProjectionMat_);

	gpuEmitter_->Update();
	//gpuParticle_->Update();

	if (!gpuEmitter_->GetIsDead()) {
		ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
		Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
		/*gpuParticle_->EmitBindCmdList(commandList, 0);*/
		gpuEmitter_->BindCmdList(commandList, 2);
		Dispatch(commandList, Vector3(1, 1, 1));
	}

	//// particleが消えた方の確認
	//if (gpuEmitter_->GetIsDead()) {
	//	CheckParticlesAlpha();
	//}

	//if (allAlphaZero_) {
	//	isAlive_ = false;
	//}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Draw() const {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	//gpuParticle_->Draw(Engine::GetCommandList());

	/*Engine::SetPipeline(PipelineType::PrimitivePipeline);
	if (!gpuEmitter_->GetIsDead()) {
		gpuEmitter_->DrawShape(viewProjectionMat_);
	}*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CSの実行命令
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group) {
	commandList->Dispatch((UINT)group.x, (UINT)group.y, (UINT)group.z);
}

//void GpuEffect::ResetParticleName(const std::string& effectName) {
//	//gpuParticle_->ResetModel(effectName.c_str());
//}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Particleがすべてきえているか確認
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::CheckParticlesAlpha() {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GpuEffect::Debug_Gui() {
	gpuEmitter_->Debug_Gui();
	gpuEmitter_->SelectEmitModel();
	if (ImGui::Button("Reset")) {
		//gpuParticle_->ResetModel(gpuEmitter_->GetParticleModel());
	}
}
#endif