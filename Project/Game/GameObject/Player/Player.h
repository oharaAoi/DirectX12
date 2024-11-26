#pragma once
#include <memory>
#include <optional>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Clutch/ClutchWire.h"
#include "Game/GameObject/Clutch/WireTip.h"
#include "Engine/Math/Easing.h"
#include "Game/GameObject/Missile.h"
#include "Game/GameObject/State/PlayerRootState.h"
#include "Game/GameObject/State/PlayerBeAttackedState.h"
#include "Game/GameObject/Player/PlayerAnimator.h"
#include "Engine/Utilities/AdjustmentItem.h"

enum class PlayerState {
	Default = 0,
	Attack = 1,
	BeAttacked,		// 攻撃を受けた状態
	Pull,			// 物を引っ張ってる状態
};

class Player
	: public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	/// <summary>
	/// 状態を遷移させる
	/// </summary>
	void CheckBehaviorRequest();

	void OnCollision(MeshCollider& other);

	/// <summary>
	/// コアに攻撃した際の跳ね返り
	/// </summary>
	void KnockBack();

	/// <summary>
	/// Bossとの距離を判定する
	/// </summary>
	void CheckBossToLength(const Vector3& bossPos);

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void AdaptAdjustment();

	void AutoMove(const Vector3& velocity);

	void SetInverMatrix(const Matrix4x4& inver);
	void SetCameraZDis(float z);

	bool GetIsKnockBack() { return isKnockBack_; }

	WireTip* GetWireTip() { return wireTip_.get(); }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	PlayerAnimator* GetPlayerAnimater() { return playerAnimator_.get(); }

	// objectを投げるように
	Vector3 GetThrowVelo()const;
	const bool& GetThrow() const { return isThrow_; }
	void SetThrow(bool is) { isThrow_ = is; }

	const bool GetPull() const { return isPull_; }
	void SetPull(bool is) { isPull_ = is; }

	// 伸びている状態かを取得
	const bool GetIsStretchClutch() const { return isStretchClutch_; }

	// 最大まで伸びて戻るかを取得
	const bool GetIsReturnClutch() const { return isReturnClutch_; }

	const bool GetCanBossAttack() const { return canBossAttack_; }

	// 右と左どちらを向いているか
	const float GetTargetRotate() const { return targetRotate; }

	void SetNearBack(bool is) { isNearBack_ = is; }
	bool GetNearBack()const { return isNearBack_; }
	bool GetPullBack()const { return isPullBackObj_; }
	void SetFalsePullBack();

	Collider* GetWireTipCollider() { return wireTip_.get(); }
	MeshCollider* GetWiretipMeshCollider() { return wireTip_->GetMeshCollider(); }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

	// playerの状態の変更
	void SetBehaviorRequest(const PlayerState& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<BaseObjectState> behaviorState) { state_ = std::move(behaviorState); }

	// 受けた攻撃の種類の取得・設定
	const BeAttackedType GetBeAttackedType() const { return beAttackedType_; }
	void SetBeAttackedType(const BeAttackedType& beAttackedType) { beAttackedType_ = beAttackedType;}

	// hpの取得・設定
	const float GetHp() const { return hp_; }
	void SetHp(float hp) { hp_ = hp; }

	const float GetHpLimit() const { return hpLimit_; }

	const bool GetIsAlive() const { return isAlive_; }

	int GetPlayerState()const { return int(playerState); }
	const bool GetIsShake()const { return isShakeBook_; }

private:

	/// ==========================================
	/// プライベート関数
	/// ==========================================
	void Move();
	void DefaultMove(Vector3& pos);
	void PullBackMove(Vector3& pos);

	void Clutch();

	/// <summary>
	/// 捕まえたオブジェクトを追従させる処理
	/// </summary>
	void CatchObjectFollow();

	void BaseClutch();
	void FirstClutch();
	void Stretching();
	void BackPullStretch();
	void ClutchEndCalculation();

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

	void DecrementHp();

private:

	/// ==========================================
	/// 変数
	/// ==========================================
	
	const std::string groupName_ = "player";

	// プレイヤーの状態
	int playerState = int(PlayerState::Default);
	// knockBack処理
	bool isKnockBack_;
	float knockBackSpeed_ = 4.0f;
	float knockBackTime_ = 0.0f;
	int knockBack_LorR_ = 1;

	// 移動
	Vector3 velocity_{};
	float moveSpeed_ = 6.0f;
	float gravity_ = -29.4f;
	float rightRotate = 1.59f;
	float leftRotate = -1.58f;
	float targetRotate = 0.0f;
	float nowRotate = 0.0f;
	float groundLine_ = 1.2f;
	bool isShakeBook_ = false;

	// クラッチ
	std::unique_ptr<ClutchWire> wire_;
	const float defaultMaxClutchLength_ = 7.0f;
	float maxClutchLength_ = defaultMaxClutchLength_;
	float stretchSpeed_ = 30.0f;
	float camerazDis_ = 0.0f;
	Vector2 clutchEnd_{};
	float clutchLerpTime_ = 0.0f;
	int easingIndex_ = int(EasingType::Out::Cubic);

	float returnSpeed_ = 0.3f;

	bool isRekey_ = true;// 一回ボタンから手離したかった

	bool isReturnClutch_ = false;// 最大まで伸びて戻るか
	bool isStretchClutch_ = false;// 伸びてる状態か

	bool isStretching_ = false;

	bool isSnagged_ = false;
	bool isPull_ = false;
	float pullWeight_ = 0.0f;
	Matrix4x4 inverMat_;


	// ワイヤー先端
	std::unique_ptr<WireTip> wireTip_;


	// 投げる
	bool isThrow_ = false;
	float throwSpeed_;

	// 奥のオブジェクト
	bool isPullBackObj_ = false;
	bool isNearBack_ = false;
	Vector3 beforePosition_{};

	/// ==========================================
	/// 攻撃に扱う情報
	/// ==========================================
	float bossAttackRange_ = 4.0f;

	bool canBossAttack_ = false;

	Vector3 bossDire_;

	/// ==========================================
	/// 状態に関する情報
	/// ==========================================
	
	std::unique_ptr<BaseObjectState> state_;
	// stateパターンに関する変数
	PlayerState behavior_ = PlayerState::Default;
	std::optional<PlayerState> behaviorRequest_ = std::nullopt;

	// 攻撃を受けた時の敵の攻撃の種類
	BeAttackedType beAttackedType_;

	bool isAlive_ = true;
	bool isAutoMove_;

	float hp_ = 10.0f;
	float hpLimit_ = 10.0f;

	const float hpDecrement_ = 1.0f;

	/// ==========================================
	/// 他クラスの情報
	/// ==========================================
	
	std::unique_ptr<PlayerAnimator> playerAnimator_ = nullptr;

	AdjustmentItem* adjustItem_;

};

