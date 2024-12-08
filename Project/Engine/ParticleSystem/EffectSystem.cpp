#include "EffectSystem.h"
#include "Engine/ParticleSystem/EffectSystemEditer.h"
#include "Engine/ParticleSystem/Emitter/Emitter.h"
#include "Engine/ParticleSystem/BaseEffect.h"

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


}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Update() {
	
#ifdef _DEBUG
	editer_->Update();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Draw() const {
#ifdef _DEBUG
	editer_->PreBegin();
	editer_->Begin();
	editer_->Draw();
#endif
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
	ImGui::Begin("EffectSystem");
	//ImGui::Checkbox("openEffectEditer", &isEffectEditer_);
	editer_->Debug_Gui();
	ImGui::End();
}

void EffectSystem::EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	editer_ = std::make_unique<EffectSystemEditer>(renderTarget, descriptorHeaps, dxCommands, device);
}
void EffectSystem::PostDraw() {
	editer_->End();
}

const bool EffectSystem::GetIsEditerFocused() const {
	return editer_->GetIsFocused();
}
#endif