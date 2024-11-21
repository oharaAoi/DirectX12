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

enum class AttackType {
	GooSlap_Attack,
	ParSlap_Attack,
	Missile_Attack,

	TOTAL
};

class BaseBossHand : public BaseGameObject {
public:

	struct AttackWork {
		float waitoTime = 2.0f;			// 攻撃までの時間
		float attackTime = 1.0f;		// 攻撃の時間
		float attackAfterTime = 1.0f;	// 攻撃後の動きの時間
		Vector3 offset;					// 座標のoffset

		bool isAttack;					// 攻撃を始めるか
		bool isAttackFinsh_;			// 攻撃を終了するか
	};

public:

	BaseBossHand();
	virtual ~BaseBossHand();

	virtual void Finalize() override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() const override;

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
	/// 手とマウスカーソルが当たっているかを確認する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="vpvpMat"></param>
	void CheckMouseCursorCollision(WorldTransform* worldTransform, const Matrix4x4& vpvpMat);

	/// <summary>
	/// 攻撃をする準備
	/// </summary>
	/// <param name="handPos"></param>
	void PrepareAttack(const AttackType& type);

	/// <summary>
	/// グーの状態でのたたきつけ
	/// </summary>
	void GooSlap();

	void Attack();

#ifdef _DEBUG
	void Debug_Draw();
#endif

public:

	using FunctionPointer = void(BaseBossHand::*)();
	std::unordered_map<AttackType, FunctionPointer> functionMap_ = {
		{AttackType::GooSlap_Attack, &BaseBossHand::GooSlap},
		{AttackType::ParSlap_Attack, &BaseBossHand::GooSlap},
	};

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	const bool GetIsAttackMove() const { return isAttackMove_; }
	void SetIsAttackMove(bool isAttack) { isAttackMove_ = isAttack; }

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

	// Animationの時間の取得・設定
	const float GetAnimationTime() const { return animationTime_; }
	void SetAnimationTime(float animeTime) { animationTime_ = animeTime; }

	// どちらの方が近いか
	const bool GetIsNear() const { return isNear_; }
	void SetIsNear(bool isNear) { isNear_ = isNear; }

	// player座標の設定
	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

	// 親となる胴体の座標を持っておく
	void SetBodyPos(const Vector3& bodyPos) { bodyPos_ = bodyPos; }

protected:

	BossAttackEditer* pAttackEditer_ = nullptr;

	Vector2 objectScreenPos_;

	Vector3 initPos_;

	uint32_t moveIndex_;
	float moveTime_;

	// -------------------------------------------------
	// ↓ 状態に関する変数
	// -------------------------------------------------

	uint32_t hp_;
	const uint32_t kDurability_ = 5;	// 耐久度

	bool isAlive_;

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
