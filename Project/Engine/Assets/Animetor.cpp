#include "Animetor.h"
#include "Engine/Assets/AnimetionClip.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/Assets/Skinning.h"

Animetor::Animetor() {
	manager_ = AnimationManager::GetInstance();
}
Animetor::~Animetor() {}

 void Animetor::Finalize() {
	skinning_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animetor::Update() {
	animetionClip_->Update();
	if (!animetionClip_->GetIsChange()) {
		animetionClip_->ApplyAnimation(skeleton_.get());
	} else {
		animetionClip_->LerpApplyAnimation(skeleton_.get());
	}

	// skinningをするなら
	if (isSkinning_) {
		skeleton_->Update();
		skinning_->Update(skeleton_.get());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animetor::LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model, bool isSkinning) {
	isSkinning_ = isSkinning;

	// -------------------------------------------------
	// ↓ animationのkeyframeを取得
	// -------------------------------------------------
	animetionClip_ = std::make_unique<AnimetionClip>();
	if (manager_->CheckAnimation(fileName)) {
		animetionClip_->LoadGetAnimation(fileName, isSkinning_);
	} else {
		animetionClip_->LoadAnimation(directoryPath, fileName, model->GetRootNodeName(), isSkinning_);
	}
	
	// -------------------------------------------------
	// ↓ skinningするのに必要な情報の取得
	// -------------------------------------------------
	if (isSkinning_) {
		skeleton_ = std::make_unique<Skeleton>();
		skeleton_->CreateSkeleton(model->GetNode());
		skeleton_->Init();
		skinning_ = Engine::CreateSkinning(skeleton_.get(), model, 0);
	}
}

#ifdef _DEBUG
void Animetor::Debug_Gui() {
	animetionClip_->Debug_Gui();
}
#endif

const bool Animetor::GetIsAnimationFinish() const {
	return animetionClip_->GetIsAnimationFinish();
}
