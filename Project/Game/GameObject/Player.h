#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/ClutchWire.h"
#include "WireTip.h"
#include "Engine/Math/Easing.h"
#include "Game/GameObject/Missile.h"

enum class PlayerState {
	Default = 0,
	Attack = 1,
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

	void OnCollision(MeshCollider& other);

	/// <summary>
	/// コアに攻撃した際の跳ね返り
	/// </summary>
	void KnockBack();

	/// <summary>
	/// Bossとの距離を判定する
	/// </summary>
	void CheckBossToLength(const Vector3& bossPos);

	void SetInverMatrix(const Matrix4x4& inver);
	void SetCameraZDis(float z);

	bool GetIsKnockBack() { return isKnockBack_; }

	WireTip* GetWireTip() { return wireTip_.get(); }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// objectを投げるように
	Vector3 GetThrowVelo()const;
	const bool& GetThrow() const { return isThrow_; }
	void SetThrow(bool is) { isThrow_ = is; }

	const bool GetPull() const { return isPull_; }
	void SetPull(bool is) { isPull_ = is; }

	void SetNearBack(bool is) { isNearBack_ = is; }
	bool GetPullBack()const { return isPullBackObj_; }
	void SetFalsePullBack();

	Collider* GetWireTipCollider() { return wireTip_.get(); }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

private:

	/// ==========================================
	/// プライベート関数
	/// ==========================================
	void Move();
	void DefaultMove(Vector3& pos);

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

	/// ==========================================
	/// 変数
	/// ==========================================

	//anime
	float animeTime_ = 0.0f;

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

	// クラッチ
	std::unique_ptr<ClutchWire> wire_;
	float maxClutchLength_ = 7.0f;
	float stretchSpeed_ = 30.0f;
	float camerazDis_ = 0.0f;
	Vector2 clutchEnd_{};
	float clutchLerpTime_ = 0.0f;
	int easingIndex_ = int(EasingType::Out::Cubic);

	float returnSpeed_ = 0.3f;
	bool isReturnClutch_ = false;// 最大まで伸びて戻るか
	bool isStretchClutch_ = false;// 伸びてる状態か
	bool isStretching_ = false;
	bool isRekey_ = true;// 一回ボタンから手離したかった
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

	/// ==========================================
	/// 攻撃に扱う情報
	/// ==========================================
	float bossAttackRange_ = 4.0f;

	bool canBossAttack_ = false;

	Vector3 bossDire_;

	/// ==========================================
	/// 他クラスの情報
	/// ==========================================

};

