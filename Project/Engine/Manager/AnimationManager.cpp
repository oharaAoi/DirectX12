#include "AnimationManager.h"

AnimationManager::~AnimationManager() {
}

AnimationManager* AnimationManager::GetInstance() {
	static AnimationManager instance;
	return &instance;
}

void AnimationManager::Init() {
}

void AnimationManager::AddMap(const AnimetionClip::Animation& animation, const std::string& name) {
	if (auto it = animationMap_.find(name); it != animationMap_.end()) {
		return;
	}

	animationMap_.try_emplace(name, animation);
}

AnimetionClip::Animation AnimationManager::GetAnimation(const std::string& name) {
	if (auto it = animationMap_.find(name); it == animationMap_.end()) {
		assert(false && "Audio not found!");
	}
	return animationMap_[name];
}

bool AnimationManager::CheckAnimation(const std::string& name) {
	if (auto it = animationMap_.find(name); it == animationMap_.end()) {
		return false;
	}

	return true;
}

