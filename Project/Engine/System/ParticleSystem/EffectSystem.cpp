#include "EffectSystem.h"
#include "Engine/System/ParticleSystem/EffectSystemEditer.h"

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
	effectList_.remove_if([this](const std::unique_ptr<GpuEffect>& effect) {
		if (!effect->GetIsAlive()) {
			return true; // 削除
		}
		return false; // 削除しない
						  }
	);

	for (std::list<std::unique_ptr<GpuEffect>>::iterator it = effectList_.begin(); it != effectList_.end();) {
		Matrix4x4 mat = Multiply(viewMat_, projectionMat_);
		(*it)->SetViewProjectionMat(mat);
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
	for (std::list<std::unique_ptr<GpuEffect>>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it)->Draw();
		++it;
	}
}

void EffectSystem::Emit(const std::string& name, const Vector3& pos) {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	for (uint32_t oi = 0; oi < persistence->GetItemsSize(name); ++oi) {
		std::string key = "emitter" + std::to_string(oi + 1);
		std::string emitterName = persistence->GetValue<std::string>(name, key.c_str());
		ImportEmitter(emitterName, pos);
	}

}

void EffectSystem::SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	viewMat_ = viewMat;
	projectionMat_ = projection;
}

void EffectSystem::ImportEmitter(const std::string& emiterName, const Vector3& pos) {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	uint32_t shape = persistence->GetValue<uint32_t>(emiterName, "shape");

	auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
	newEffect->Init(static_cast<EmitterShape>(shape));
	newEffect->SetEmitter(emiterName);
	newEffect->SetEmitterPos(pos);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Syetemのための関数群
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void EffectSystem::Debug_Gui() {
	editer_->Debug_Gui();
#ifdef _DEBUG
	editer_->PreBegin();
	editer_->Begin();
	editer_->Draw();
#endif
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