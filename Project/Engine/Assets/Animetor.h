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

	void LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model, bool isSkinning);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	Skinning* GetSkinning() { return skinning_.get(); }

	const Matrix4x4 GetAnimationMat() const { return animetionClip_->GetMatrix(); }

	const bool GetIsAnimationFinish() const;

	float GetAnimationTime() const { return animetionClip_->GetAnimationTime(); }
	void SetAnimationTime(float time) { animetionClip_->SetAnimationTime(time); }

	const bool GetIsSkinning() const { return isSkinning_; }

private:

	AnimationManager* manager_;

	std::unique_ptr<AnimetionClip> animetionClip_;
	std::unique_ptr<Skeleton> skeleton_ = nullptr;
	std::unique_ptr<Skinning> skinning_ = nullptr;

	bool isSkinning_;
};

