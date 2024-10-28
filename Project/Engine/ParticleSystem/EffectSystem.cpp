#include "EffectSystem.h"
#include "Engine/ParticleSystem/EffectSystemEditer.h"
#include "Engine/ParticleSystem/Emitter.h"
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
	/*particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();*/
	/*particleField_->SetParticle(particle_.get());*/

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

	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin();
			 effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Finalize();

			++effectListIter;
		}
		++effectDataListIter;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Update() {
	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<Emitter>>::iterator emitterListIter = effectDataListIter->emitterList.begin(); emitterListIter != effectDataListIter->emitterList.end();) {
			if ((*emitterListIter)->GetIsDead()) {
				emitterListIter = effectDataListIter->emitterList.erase(emitterListIter);
			}
			(*emitterListIter)->Update();
			++emitterListIter;
		}

		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin(); effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->SetCameraMatrix(cameraMat_);
			(*effectListIter)->Update(viewMat_, projectionMat_);
			++effectListIter;
		}

		++effectDataListIter;
	}

	// -------------------------------------------------
	// ↓ 当たり判定の実装
	// -------------------------------------------------
	//particleField_->Update();

	// -------------------------------------------------
	// ↓ ImGuiの描画
	// -------------------------------------------------
	//particle_->ImGuiDraw();
#ifdef _DEBUG
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::Draw() const {
	for (std::list<EffectData>::const_iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::const_iterator effectListIter = effectDataListIter->effectList.begin();
			 effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Draw();
			++effectListIter;
		}
		
		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<Emitter>>::const_iterator emitterListIter = effectDataListIter->emitterList.begin();
			 emitterListIter != effectDataListIter->emitterList.end();) {
			(*emitterListIter)->Draw(viewMat_ * projectionMat_);
			++emitterListIter;
		}

		++effectDataListIter;
	}
	//particleField_->Draw(viewMat_ * projectionMat_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::CreateEffect() {
	EffectData effectData;
	std::unique_ptr<BaseEffect> effect = std::make_unique<BaseEffect>();
	effect->Init("./Resources/Effect/", "particle.obj", 10);
	std::unique_ptr<Emitter> emitter = std::make_unique<Emitter>(effect.get());
	
	effectData.emitterList.push_back(std::move(emitter));
	effectData.effectList.push_back(std::move(effect));

	effectList_.push_back(std::move(effectData));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Emitterのファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystem::LoadEmitter(const std::string& fileName) {
	std::string filePath = kDirectoryPath_ + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);
	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
	}

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
	/*ImGui::Begin("Engine");
	ImGui::Checkbox("openEffectEditer", &isEffectEditer_);
	ImGui::End();*/
}

void EffectSystem::EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	editer_ = std::make_unique<EffectSystemEditer>(renderTarget, descriptorHeaps, dxCommands, device);
}

void EffectSystem::BeginEditer() {
	editer_->Begin();
}

void EffectSystem::UpdateEditer() {
	editer_->Update();
}

void EffectSystem::DrawEditer() {
	editer_->Draw();
}

void EffectSystem::EndEditer() {
	editer_->End();
}

#endif