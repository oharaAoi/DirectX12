#pragma once
#include <memory>
#include "Game/GameObject/BossBody.h"
#include "Game/GameObject/BossLeftHand.h"
#include "Game/GameObject/BossRightHand.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Game/GameObject/State/BossRootState.h"
#include "Game/GameObject/State/BossAttackState.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// <summary>
/// Bossの大元となるクラス
/// </summary>
class Boss {
public:

	Boss();
	~Boss();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;

	void CheckMouseCursolCollision(const Matrix4x4& vpvpMat);

	// ------------------- アクセッサ ------------------- //

	void SetEditer(BossAttackEditer* left, BossAttackEditer* right) {
		leftHandEditer_ = left;
		rightHandEditer_ = right;
		boss_leftHand_->SetAttackEditer(left);
		boss_rightHand_->SetAttackEditer(right);

		boss_leftHand_->LoadAllFile();
		boss_rightHand_->LoadAllFile();
	}

#ifdef _DEBUG
	void Debug_Gui();

	void Save();
private:

	std::string leftImportFile_;
	std::string rightImportFile_;

	uint32_t leftImportIndex_;
	uint32_t rightImportIndex_;

#endif

private:

	// 体
	std::unique_ptr<BossBody> boss_body_;
	// 左手
	std::unique_ptr<BossLeftHand> boss_leftHand_;
	// 右手
	std::unique_ptr<BossRightHand> boss_rightHand_;

	BossAttackEditer* leftHandEditer_ = nullptr;
	BossAttackEditer* rightHandEditer_ = nullptr;

	// -------------------------------------------------
	// ↓ 攻撃に関する変数
	// -------------------------------------------------

	std::string attackName_;
	const std::string attackDirectoryPath_ = "./Game/Resources/GameData/BossAttack/Attacks/";

	// 攻撃に関するファイルをまとめた配列
	std::vector<std::string> attackFileNames_;

	// -------------------------------------------------
	// ↓ 状態に関する変数
	// -------------------------------------------------

	std::unique_ptr<BaseObjectState> state_;
};
