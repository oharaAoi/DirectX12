#pragma once
#include <memory>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Manager/AnimationManager.h"

class AnimetionClip;
class Skeleton;
class Skinning;

/// <summary>
/// SkeletonやSkinningの情報を持っているクラス
/// </summary>
class Animetor {
public:

	Animetor();
	~Animetor();

	void Finalize();
	void Update();

	void UpdateScript();

	void LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model);

public:

	Skinning* GetSkinning() { return skinning_.get(); }

	const bool GetIsAnimationFinish() const;

	float GetAnimationTime() const { return animetionClip_->GetAnimationTime(); }
	void SetAnimationTime(float time) { animetionClip_->SetAnimationTime(time); }

private:

	AnimationManager* manager_;

	std::unique_ptr<AnimetionClip> animetionClip_;
	std::unique_ptr<Skeleton> skeleton_ = nullptr;
	std::unique_ptr<Skinning> skinning_ = nullptr;
};

