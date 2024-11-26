#pragma once
#include <memory>
#include <optional>
#include "Game/GameObject/Boss/BossBody.h"
#include "Game/GameObject/Boss/BossCore.h"
#include "Game/GameObject/Boss/BossLeftHand.h"
#include "Game/GameObject/Boss/BossRightHand.h"
#include "Game/GameObject/Boss/BossBarrier.h"
#include "Game/GameObject/Boss/BossEye.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Game/GameObject/State/BaseObjectState.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GameScene;

enum class Behavior{
	ROOT,		// 待機状態
	ATTACK,		// 攻撃状態
	TRANSITION,	// 遷移状態
	APPEAR,		// 登場状態
};

enum BossForm {
	FIRST,
	SECOND
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
	void PostDraw() const;

	/// <summary>
	/// 2d描画
	/// </summary>
	void DrawUI() const;

	void CheckBehaviorRequest();

	void CheckMouseCursolCollision(const Matrix4x4& vpvpMat);

	void CheckAttackType(const AttackType& type);

	void MissileAttack();

	void RecoveryHp();

public:

	// ------------------- アクセッサ ------------------- //

	void SetEditer(BossAttackEditer* left, BossAttackEditer* right);

	BossCore* GetBossCore() { return core_.get(); }
	BossBody* GetBossBody() { return body_.get(); }
	BossLeftHand* GetBossLeftHand() { return leftHand_.get(); }
	BossRightHand* GetBossRightHand() { return rightHand_.get(); }
	BossEye* GetBossEye() { return eye_.get(); }
	BossBarrier* GetBossBarrier() { return barrier_.get(); }

	const Vector3 GetBossBodyPos() const { return body_->GetTransform()->GetTranslation(); }

	// BossのHpの取得・設定
	const float GetBossHp() const { return bossHp_; }
	void SetBossHp(float newHp) { bossHp_ = newHp; };

	// 第二形態に遷移するフラグの取得設定
	const bool GetIsTransitionForm() const { return isTransitionForm_; }
	void SetIsTransitionForm(bool isTransition) { isTransitionForm_ = isTransition; }

	// 登場に関するフラグ
	const bool GetIsAppear() const { return isAppear_; }
	void SetIsAppear(bool isAppear) { isAppear_ = isAppear; }

	// 今の形態
	const BossForm GetBossForm() const { return form_; }
	void SetBossForm(BossForm form) { form_ = form; }

	const bool GetIsAlive() const { return isAlive_; }

	void SetIsRecovery() { isRecovery_ = true; }

	// playerの座標を設定
	const Vector3 GetPlayerPos() const { return playerPos_; }
	void SetPlayerPos(const Vector3& playerPos) {
		playerPos_ = playerPos;
		leftHand_->SetPlayerPos(playerPos);
		rightHand_->SetPlayerPos(playerPos);
	}

	// Behaviorの次の状態をリクエストする・状態を設定する
	void SetBehaviorRequest(const Behavior& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<BaseObjectState> behaviorState) { state_ = std::move(behaviorState); }

	// 攻撃する種類を取得
	const AttackType GetAttackType() const { return attackType_; }
	void SetAttackType(const AttackType& type) { attackType_ = type; }

	// gameSceneのセット
	void SetGameScene(GameScene* gameScene) { pGameScene_ = gameScene; }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();

	void Save();
private:

	std::string leftImportFile_;
	std::string rightImportFile_;

	uint32_t leftImportIndex_;
	uint32_t rightImportIndex_;

	bool isSetAttack_ = false;

#endif

private:

	// 体
	std::unique_ptr<BossBody> body_;
	// コア部
	std::unique_ptr<BossCore> core_;
	// 左手
	std::unique_ptr<BossLeftHand> leftHand_;
	// 右手
	std::unique_ptr<BossRightHand> rightHand_;
	// バリア
	std::unique_ptr<BossBarrier> barrier_;
	// 目
	std::unique_ptr<BossEye> eye_;

	BossAttackEditer* leftHandEditer_ = nullptr;
	BossAttackEditer* rightHandEditer_ = nullptr;

	// -------------------------------------------------
	// ↓ 攻撃に関する変数
	// -------------------------------------------------

	AttackType attackType_;
	int attackTypeNum_;

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

	bool isAlive_ = true;
	float bossHp_ = 100;

	// 第二形態に遷移する
	bool isTransitionForm_;
	BossForm form_;

	bool isRecovery_ = false;

	// -------------------------------------------------
	// ↓ Game開始時に必要な情報
	// -------------------------------------------------

	bool isAppear_; // 登場する状態か

	// -------------------------------------------------
	// ↓ Boss以外の情報
	// -------------------------------------------------
	Vector3 playerPos_;

	std::string near_;

	GameScene* pGameScene_;

};

