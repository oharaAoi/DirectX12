#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Game/Editer/BossAttackEditer.h"

class BossLeftHand : public BaseGameObject {
public:

	BossLeftHand();
	~BossLeftHand();

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

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

private:

	BossAttackEditer* pAttackEditer_ = nullptr;

	std::string groupName_ = "BossLeftHand";
	std::string attackDirectoryPath = "./Game/Resources/GameData/BossAttack/LeftHand/";

	std::unique_ptr<MeshCollider> meshCollider_;

	Vector2 objectScreenPos_;

	uint32_t moveIndex_;
	float moveTime_;

	// 攻撃に使う変数

	// フラグ
	bool isAttackMove_;	// 攻撃の動きをするか
	bool isClicked_;	// クリックされているか

};

