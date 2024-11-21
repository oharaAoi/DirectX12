#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/Easing.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Game/GameObject/Boss/BaseBossHand.h"

/// <summary>
/// Bossの右手
/// </summary>
class BossRightHand : public BaseBossHand {
public:

	BossRightHand();
	~BossRightHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void LoadAllFile();

	void AnimationTimeIncrement(bool isLoop) { AnimeTimeIncrement(isLoop, animetor_->GetAnimationDuration()); };

public:

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "BossRightHand";
	std::string attackDirectoryPath = "./Game/Resources/GameData/BossAttack/RightHand/";
	std::string animationDirectoryPath = "./Game/Resources/GameData/BossAnimation/RightHand/";
};

