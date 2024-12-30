#include "AnimationManager.h"

AnimationManager::~AnimationManager() {
}

AnimationManager* AnimationManager::GetInstance() {
	static AnimationManager instance;
	return &instance;
}

void AnimationManager::Init() {
}

void AnimationManager::AddMap(const std::unordered_map<std::string, AnimationClip::Animation>& animation, const std::string& name) {
	if (auto it = animationMap_.find(name); it != animationMap_.end()) {
		return;
	}

	// ファイルに格納されているアニメーションの名前を格納する
	for (auto animationName : animation) {
		modelHaveAnimationNames_[name].push_back(animationName.first);
	}

	animationMap_.try_emplace(name, animation);
}

AnimationClip::Animation AnimationManager::GetAnimation(const std::string& animationFile, const std::string& animationName) {
	if (auto it = animationMap_.find(animationFile); it == animationMap_.end()) {
		assert(false && "Animation not found!");
	}

	return animationMap_[animationFile][animationName];
}

std::string AnimationManager::GetAnimationFirstName(const std::string& animationFileName) {
	return modelHaveAnimationNames_[animationFileName][0];
}

bool AnimationManager::CheckAnimation(const std::string& name) {
	if (auto it = animationMap_.find(name); it == animationMap_.end()) {
		return false;
	}

	return true;
}

