#pragma once
#include <string>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Math/Easing.h"
#include "Game/Editer/BossAttackEditer.h"
#include "Game/Action/IAttack.h"
#include "Engine/2d/Sprite.h"
#include "Game/GameObject/Player/Player.h"

class BossGooAttack;

enum class AttackType {
	GooSlap_Attack,
	ParSlap_Attack,
	Missile_Attack,
	// 2形態以降
	MowDown_Attack,
	TOTAL
};

enum class HandType {
	LEFT_HAND,
	RIGHT_HAND,
};

class BaseBossHand : public BaseGameObject {
public:

	friend class BossGooAttack;
	friend class BossParAttack;
	friend class BossMowDownAttack;

	struct AttackWork {
		float waitoTime = 2.0f;			// 攻撃までの時間
		float attackTime = 1.0f;		// 攻撃の時間
		float attackAfterTime = 1.0f;	// 攻撃後の動きの時間
		Vector3 offset;					// 座標のoffset

		bool isAttack;					// 攻撃を始めるか
		bool isAttackFinsh_;			// 攻撃を終了するか

		int easeFunction;				// イージングの種類
	};

public:

	BaseBossHand();
	virtual ~BaseBossHand();

	virtual void Finalize() override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() const override;

	void DrawUI() const;

	/// <summary>
	/// Animationの時間を増加させる
	/// </summary>
	/// <param name="isLoop">: ループさせるか</param>
	/// <param name="limitTime">: 上限の時間</param>
	void AnimeTimeIncrement(bool isLoop, float limitTime);

	/// <summary>
	/// 攻撃の動きを行う
	/// </summary>
	/// <param name="worldTransform"></param>
	void AttackMove(WorldTransform* worldTransform);

	/// <summary>
	/// 待機モーションに遷移させる
	/// </summary>
	void ChangeRootMove(BaseGameObject* object);

	/// <summary>
	/// 攻撃をする準備
	/// </summary>
	/// <param name="handPos"></param>
	void PrepareAttack(const AttackType& type);

	// 攻撃する
	void Attack();

	/// <summary>
	/// 手とマウスカーソルが当たっているかを確認する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="vpvpMat"></param>
	void CheckMouseCursorCollision(WorldTransform* worldTransform, const Matrix4x4& vpvpMat);

	void SetIsGroundSlap(bool is) { isGroundSlap_ = is; }
	const bool GetIsGroundSlap() const { return isGroundSlap_; }


#ifdef _DEBUG
	void Debug_Draw();
#endif

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	IAttack* GetIAttack() { return attackAction_.get(); }

	const bool GetIsAttackMove() const { return isAttackMove_; }
	void SetIsAttackMove(bool isAttack) { isAttackMove_ = isAttack; }

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

	// Animationの時間の取得・設定
	const float GetAnimationTime() const { return animationTime_; }
	void SetAnimationTime(float animeTime) { animationTime_ = animeTime; }

	// どちらの方が近いか
	const bool GetIsNear() const { return isNear_; }
	void SetIsNear(bool isNear) { isNear_ = isNear; }

	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

	// player座標の設定
	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

	// 親となる胴体の座標を持っておく
	void SetBodyPos(const Vector3& bodyPos) { bodyPos_ = bodyPos; }

protected:

	BossAttackEditer* pAttackEditer_ = nullptr;

	Vector2 objectScreenPos_;

	Vector3 initPos_;

	Quaternion localRotate_;

	uint32_t moveIndex_;

	HandType handType_;

	// Hp表記
	std::unique_ptr<Sprite> hpSprite_;
	float hpSpriteDisplayTime_;
	const float hpSpriteDisplayTimeLimit_ = 2.0f;

	// -------------------------------------------------
	// ↓ 状態に関する変数
	// -------------------------------------------------

	uint32_t hp_;
	uint32_t preHp_;
	const uint32_t kDurability_ = 1;	// 耐久度

	bool isAlive_;
	bool isExplosion_;
	Vector3 fallVelocity_;
	Vector3 fallAcceleration_ = Vector3(0.0f, -1.0f, 0.0f);

	// -------------------------------------------------
	// ↓ ファイルに保存する変数
	// -------------------------------------------------
	float moveTimeLimit_;	// 動く時間
	int easeType_;			// easingの種類

	// -------------------------------------------------
	// ↓ 攻撃に使う変数
	// -------------------------------------------------
	AttackType attackType_;
	AttackWork attackWork_;

	Vector3 beforeAttackPos_; // 攻撃する前の座標
	Vector3 attackVeclocity_; // 攻撃を行う方向
	float attackSpeed_ = 20.0f;		  // 攻撃を行う速度
	int easingIndex_ = (int)EasingType::Out::Quart;		  // 攻撃をする際のイージング

	std::unique_ptr<IAttack> attackAction_;

	// -------------------------------------------------
	// ↓ フラグ
	// -------------------------------------------------
	bool isAttackMove_;	// 攻撃の動きをするか
	bool isClicked_;	// クリックされているか
	bool isGroundSlap_;	// 地面をたたいたか

	bool isNear_;		// 右手と左手でどちらの方が近いかのフラグ

	// -------------------------------------------------
	// ↓ Animationに関する変数
	// -------------------------------------------------
	float animationTime_;
	float animationTransitionTime_ = 1.0f;

	std::string nowAnimatonName_;// 今のanimationの名前
	std::string waitAnimationName_;// 待機時のanimationの名前

	bool isIncrement_ = false;

	// -------------------------------------------------
	// ↓ 他クラスの情報
	// -------------------------------------------------

	Vector3 playerPos_;

	Vector3 bodyPos_;

};
