#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Engine/Math/Easing.h"
#include "Game/GameObject/BaseBossHand.h"

class BossLeftHand : public BaseBossHand {
public:

	BossLeftHand();
	~BossLeftHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void LoadAllFile();

private:

	void AnimationTimeIncrement(bool isLoop) { AnimeTimeIncrement(isLoop, animetor_->GetAnimationDuration()); };

public:

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

private:

	std::string groupName_ = "BossLeftHand";
	std::string attackDirectoryPath = "./Game/Resources/GameData/BossAttack/LeftHand/";
	std::string animationDirectoryPath = "./Game/Resources/GameData/BossAnimation/LeftHand/";
};

