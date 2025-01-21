#include "Animator.h"
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
	if (isSkinning_) {
		if (!animationClip_->GetIsChange()) {
			animationClip_->ApplyAnimation(skeleton_.get());
		} else {
			animationClip_->LerpApplyAnimation(skeleton_.get());
		}
	}

	UpdateSkinning();
}


void Animator::UpdateScript(float& animationTime) {
	if (!animationClip_->GetIsChange()) {
		// Animationの遷移がなかったらそのままタイムの更新を行う
		animationClip_->SetAnimationTime(animationTime);
		animationClip_->ApplyAnimation(skeleton_.get());
	} else {
		animationClip_->LerpApplyAnimation(skeleton_.get());
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
	animationClip_->Init(model->GetRootNodeName(), isSkinning_, isLoop);
	animationClip_->LoadAnimation(directoryPath, fileName);
	
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Animationの遷移
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animator::TransitionAnimation(const std::string& preAnimation, const std::string& afterAnimation, float blendSpeed) {
	animationClip_->LerpAnimation(preAnimation, afterAnimation, blendSpeed);
}

void Animator::TransitionAnimation(const std::string& afterAnimation, float blendSpeed) {
	animationClip_->LerpAnimation(afterAnimation, blendSpeed);
}

#ifdef _DEBUG
void Animator::Debug_Gui() {
	animationClip_->Debug_Gui();
}
#endif
