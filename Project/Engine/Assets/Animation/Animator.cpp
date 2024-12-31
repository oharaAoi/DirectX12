<<<<<<<< HEAD:Project/Engine/Assets/Animation/Animetor.cpp
#include "Animetor.h"
========
#include "Animator.h"
>>>>>>>> main:Project/Engine/Assets/Animation/Animator.cpp
#include "Engine/Assets/Animation/AnimationClip.h"
#include "Engine/Assets/Rigging/Skeleton.h"
#include "Engine/Assets/Rigging/Skinning.h"

Animator::Animator() {
	manager_ = AnimationManager::GetInstance();
}
Animator::~Animator() {}

 void Animator::Finalize() {
	skinning_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animator::Update() {
	// scriptでanimationの時間を制御して入なかったら自動的に更新
	if (!isControlScript_) {
		animationClip_->Update();
	}

	// animationの遷移がなかったらそのままanimationさせる
	if (!animationClip_->GetIsChange()) {
		animationClip_->ApplyAnimation(skeleton_.get());
	} else {
		animationClip_->LerpApplyAnimation(skeleton_.get());
	}

	UpdateSkinning();
}


<<<<<<<< HEAD:Project/Engine/Assets/Animation/Animetor.cpp
void Animetor::UpdateScript(float& animationTime, float transitionTime) {
========
void Animator::UpdateScript(float& animationTime, float transitionTime) {
>>>>>>>> main:Project/Engine/Assets/Animation/Animator.cpp
	if (!animationClip_->GetIsChange()) {
		// Animationの遷移がなかったらそのままタイムの更新を行う
		animationClip_->SetAnimationTime(animationTime);
		animationClip_->ApplyAnimation(skeleton_.get());
	} else {
		animationClip_->AnimationTransition(skeleton_.get(), transitionTime);
		// 更新を行ってAnimationが完全に切り替わったら遷移を終了しAnimationの時間を合うようにする
		if (!animationClip_->GetIsChange()) {
			animationTime = animationClip_->GetAnimationTime();
		}
	}

	UpdateSkinning();
}


void Animator::UpdateSkinning() {
	// skinningをするなら
	if (isSkinning_) {
		skeleton_->Update();
		skinning_->Update(skeleton_.get());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animator::LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model, bool isSkinning, bool isLoop, bool isControlScript) {
	isSkinning_ = isSkinning;
	isControlScript_ = isControlScript;

	// -------------------------------------------------
	// ↓ animationのkeyframeを取得
	// -------------------------------------------------
	animationClip_ = std::make_unique<AnimationClip>();
	if (manager_->CheckAnimation(fileName)) {
		animationClip_->LoadGetAnimation(fileName, isSkinning_);
	} else {
		animationClip_->LoadAnimation(directoryPath, fileName, model->GetRootNodeName(), isSkinning_);
	}
	animationClip_->SetIsLoop(isLoop);
	
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

<<<<<<<< HEAD:Project/Engine/Assets/Animation/Animetor.cpp
void Animetor::TransitionAnimation(const std::string& preAnimation, const std::string& afterAnimation) {
	animationClip_->SetLerpAnimation(preAnimation, afterAnimation);
}

void Animetor::TransitionAnimation(const std::string& afterAnimation) {
========
void Animator::TransitionAnimation(const std::string& preAnimation, const std::string& afterAnimation) {
	animationClip_->SetLerpAnimation(preAnimation, afterAnimation);
}

void Animator::TransitionAnimation(const std::string& afterAnimation) {
>>>>>>>> main:Project/Engine/Assets/Animation/Animator.cpp
	animationClip_->SetLerpAnimation(afterAnimation);
}

#ifdef _DEBUG
<<<<<<<< HEAD:Project/Engine/Assets/Animation/Animetor.cpp
void Animetor::Debug_Gui() {
========
void Animator::Debug_Gui() {
>>>>>>>> main:Project/Engine/Assets/Animation/Animator.cpp
	animationClip_->Debug_Gui();
}
#endif
