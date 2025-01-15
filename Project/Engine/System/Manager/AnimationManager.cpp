#include "AnimationManager.h"
#include "Engine/Utilities/Loader.h"

AnimationManager::~AnimationManager() {
}

AnimationManager* AnimationManager::GetInstance() {
	static AnimationManager instance;
	return &instance;
}

void AnimationManager::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animaitonを読み込んでいるかを確認する
//////////////////////////////////////////////////////////////////////////////////////////////////

bool AnimationManager::CheckAnimationMap(const std::string& animationFile) {
	if (auto it = animationMap_.find(animationFile); it == animationMap_.end()) {
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Animationのファイルを読み込む
//////////////////////////////////////////////////////////////////////////////////////////////////

Animation AnimationManager::LoadAnimationFile(const std::string& directory, const std::string& animationFile) {
	AddMap(LoadAnimation(directory, animationFile), animationFile);
	return animationMap_[animationFile][GetAnimationFirstName(animationFile)];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　AnimationのMapをMapに追加する
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationManager::AddMap(const std::unordered_map<std::string, Animation>& animation, const std::string& animationFileName) {
	if (auto it = animationMap_.find(animationFileName); it != animationMap_.end()) {
		return;
	}

	// ファイルに格納されているアニメーションの名前を格納する
	for (auto animationName : animation) {
		modelHaveAnimationNames_[animationFileName].push_back(animationName.first);
	}

	animationMap_.try_emplace(animationFileName, animation);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Animationをmapから取得する
//////////////////////////////////////////////////////////////////////////////////////////////////

Animation AnimationManager::GetAnimation(const std::string& fileName, const std::string& animationName) {
	// mapに存在していなかったら読み込んでmapに追加する
	if (auto it = animationMap_.find(fileName); it == animationMap_.end()) {
		assert(false && "animationFile not Load!");
	}

	return animationMap_[fileName][animationName];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationのmapに入っている最初のAnimation名を取得する
//////////////////////////////////////////////////////////////////////////////////////////////////

std::string AnimationManager::GetAnimationFirstName(const std::string& animationFileName) {
	return modelHaveAnimationNames_[animationFileName][0];
}

bool AnimationManager::CheckAnimation(const std::string& name) {
	if (auto it = animationMap_.find(name); it == animationMap_.end()) {
		return false;
	}

	return true;
}

