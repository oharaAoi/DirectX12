#include "EffectSystem.h"
#include "Engine/ParticleSystem/EffectSystemEditer.h"

EffectSystem::~EffectSystem() {}

EffectSystem* EffectSystem::GetInstacne() {
	static EffectSystem instance;
	return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Init() {
	particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();

	effectPersistence_ = EffectPersistence::GetInstance();
	effectPersistence_->Init();

	// -------------------------------------------------
	// ↓ Systemの初期化
	// -------------------------------------------------
	dxCommon_ = DirectXCommon::GetInstacne();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Finalize() {
#ifdef _DEBUG
	editer_->Finalize();
#endif
	effectList_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Update() {
	for (std::list<std::unique_ptr<GpuEffect>>::iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->SetViewProjectionMat(viewMat_ * projectionMat_);
		(*it)->Update();
		++it;
	}

#ifdef _DEBUG
	editer_->Update();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (std::list<std::unique_ptr<GpuEffect>>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->Draw();
		++it;
	}

#ifdef _DEBUG
	editer_->PreBegin();
	editer_->Begin();
	editer_->Draw();
#endif
}

void EffectSystem::Emit(const std::string& name, const Vector3& pos, const Vector4& color) {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	uint32_t shape = persistence->GetValue<uint32_t>(name, "shape");

	auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
	newEffect->Init(static_cast<EmitterShape>(shape));
	newEffect->SetEmitter(name);

	newEffect->SetEmitterPos(pos);
	newEffect->SetEmitterColor(color);
}

void EffectSystem::SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	viewMat_ = viewMat;
	projectionMat_ = projection;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Syetemのための関数群
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void EffectSystem::Debug_Gui() {
	editer_->Debug_Gui();
}

void EffectSystem::EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	editer_ = std::make_unique<EffectSystemEditer>(renderTarget, descriptorHeaps, dxCommands, device);
}
void EffectSystem::EndEditer() {
	editer_->End();
}

const bool EffectSystem::GetIsEditerFocused() const {
	return editer_->GetIsFocused();
}
#endif