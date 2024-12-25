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
	// scriptでanimationの時間を制御して入なかったら自動的に更新
	if (!isControlScript_) {
		animetionClip_->Update();
	}

	// animationの遷移がなかったらそのままanimationさせる
	if (!animetionClip_->GetIsChange()) {
		animetionClip_->ApplyAnimation(skeleton_.get());
	} else {
		animetionClip_->LerpApplyAnimation(skeleton_.get());
	}

	UpdateSkinning();
}


void Animetor::UpdateScript(float& animationTime, float transitionTime) {
	if (!animetionClip_->GetIsChange()) {
		// Animationの遷移がなかったらそのままタイムの更新を行う
		animetionClip_->SetAnimationTime(animationTime);
		animetionClip_->ApplyAnimation(skeleton_.get());
	} else {
		animetionClip_->AnimationTransition(skeleton_.get(), transitionTime);
		// 更新を行ってAnimationが完全に切り替わったら遷移を終了しAnimationの時間を合うようにする
		if (!animetionClip_->GetIsChange()) {
			animationTime = animetionClip_->GetAnimationTime();
		}
	}

	UpdateSkinning();
}


void Animetor::UpdateSkinning() {
	// skinningをするなら
	if (isSkinning_) {
		skeleton_->Update();
		skinning_->Update(skeleton_.get());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animetor::LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model, bool isSkinning, bool isLoop, bool isControlScript) {
	isSkinning_ = isSkinning;
	isControlScript_ = isControlScript;

	// -------------------------------------------------
	// ↓ animationのkeyframeを取得
	// -------------------------------------------------
	animetionClip_ = std::make_unique<AnimetionClip>();
	if (manager_->CheckAnimation(fileName)) {
		animetionClip_->LoadGetAnimation(fileName, isSkinning_);
	} else {
		animetionClip_->LoadAnimation(directoryPath, fileName, model->GetRootNodeName(), isSkinning_);
	}
	animetionClip_->SetIsLoop(isLoop);
	
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

void Animetor::TransitionAnimation(const std::string& preAnimation, const std::string& afterAnimation) {
	animetionClip_->SetLerpAnimation(preAnimation, afterAnimation);
}

void Animetor::TransitionAnimation(const std::string& afterAnimation) {
	animetionClip_->SetLerpAnimation(afterAnimation);
}

#ifdef _DEBUG
void Animetor::Debug_Gui() {
	animetionClip_->Debug_Gui();
}
#endif
