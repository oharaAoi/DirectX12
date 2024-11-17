#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Engine/Math/Easing.h"
#include "Game/Editer/BossAttackEditer.h"

/// <summary>
/// Bossの右手
/// </summary>
class BossRightHand : public BaseGameObject {
public:

	BossRightHand();
	~BossRightHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void LoadAllFile();

	void AttackMove();

	void CheckMouseCursorCollision(const Matrix4x4& vpvpMat);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsAttack() const { return isAttackMove_; }
	void SetIsAttack(bool isAttack) { isAttackMove_ = isAttack; }

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

private:

	BossAttackEditer* pAttackEditer_ = nullptr;

	std::string groupName_ = "BossRightHand";
	std::string attackDirectoryPath = "./Game/Resources/GameData/BossAttack/RightHand/";
	std::string animationDirectoryPath = "./Game/Resources/GameData/BossAnimation/RightHand/";

	Vector2 objectScreenPos_;

	uint32_t moveIndex_;
	float moveTime_;

	// ファイルに保存する変数
	float moveTimeLimit_;	// 動く時間
	int easeType_;			// easingの種類

	// 攻撃に使う変数
	
	// フラグ
	bool isAttackMove_;	// 攻撃の動きをするか
	bool isClicked_;	// クリックされているか

	// Animationに関する変数
	float animationTime_;
	float animationTransitionTime_;
};

