#pragma once
#include <memory>
#include <optional>
#include "Game/GameObject/BossBody.h"
#include "Game/GameObject/BossCore.h"
#include "Game/GameObject/BossLeftHand.h"
#include "Game/GameObject/BossRightHand.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Game/GameObject/State/BossRootState.h"
#include "Game/GameObject/State/BossAttackState.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class Behavior{
	ROOT,
	ATTACK
};

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

	void CheckBehaviorRequest();

	void CheckMouseCursolCollision(const Matrix4x4& vpvpMat);

public:

	// ------------------- アクセッサ ------------------- //

	void SetEditer(BossAttackEditer* left, BossAttackEditer* right) {
		leftHandEditer_ = left;
		rightHandEditer_ = right;
		boss_leftHand_->SetAttackEditer(left);
		boss_rightHand_->SetAttackEditer(right);

		boss_leftHand_->LoadAllFile();
		boss_rightHand_->LoadAllFile();

		if (boss_leftHand_->GetAnimetor() != nullptr) {
			leftHandEditer_->SetAnimations(boss_leftHand_->GetAnimetor()->GetAnimationNames());
		}
		if (boss_rightHand_->GetAnimetor() != nullptr) {
			rightHandEditer_->SetAnimations(boss_rightHand_->GetAnimetor()->GetAnimationNames());
		}
	}

	BossCore* GetBossCore() { return boss_core_.get(); }
	BossLeftHand* GetBossLeftHand() { return boss_leftHand_.get(); }
	BossRightHand* GetBossRightHand() { return boss_rightHand_.get(); }

	// BossのHpの取得・設定
	const float GetBossHp() const { return bossHp_; }
	void SetBossHp(float newHp) { bossHp_ = newHp; };

	// playerの座標を設定
	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

	// Behaviorの次の状態をリクエストする・状態を設定する
	void SetBehaviorRequest(const Behavior& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<BaseObjectState> behaviorState) { state_ = std::move(behaviorState); }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();

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
	// コア部
	std::unique_ptr<BossCore> boss_core_;
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
	// stateパターンに関する変数
	Behavior behavior_ = Behavior::ROOT;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	float bossHp_ = 100;

	// -------------------------------------------------
	// ↓ Boss以外の情報
	// -------------------------------------------------
	Vector3 playerPos_;

};

