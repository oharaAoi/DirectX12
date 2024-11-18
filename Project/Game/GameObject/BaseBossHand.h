#pragma once
#include <string>
#include <memory>
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Math/Easing.h"
#include "Game/Editer/BossAttackEditer.h"

enum class AttackType {
	GooSlap_Attack,
	ParSlap_Attack,
};

class BaseBossHand {
public:

	struct AttackWork {
		float waitoTime = 2.0f;		// 攻撃までの時間
		float attackTime = 1.0f;	// 攻撃の時間
		Vector3 offset;				// 座標のoffset

		bool isAttack;				// 攻撃を始めるか
		bool isAttackFinsh_;		// 攻撃を終了するか
	};

public:

	BaseBossHand();
	virtual ~BaseBossHand();

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
	/// グーの状態でのたたきつけ
	/// </summary>
	void GooSlap(WorldTransform* worldTransform, const Vector3& playerPos);

public:

	const bool GetIsAttack() const { return isAttackMove_; }
	void SetIsAttack(bool isAttack) { isAttackMove_ = isAttack; }

	void SetAttackEditer(BossAttackEditer* pRightHandEditer) { pAttackEditer_ = pRightHandEditer; }

	// Animationの時間の取得・設定
	const float GetAnimationTime() const { return animationTime_; }
	void SetAnimationTime(float animeTime) { animationTime_ = animeTime; }

protected:

	BossAttackEditer* pAttackEditer_ = nullptr;

	Vector2 objectScreenPos_;

	uint32_t moveIndex_;
	float moveTime_;

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
	int easingIndex_;		  // 攻撃をする際のイージング

	// -------------------------------------------------
	// ↓ フラグ
	// -------------------------------------------------
	bool isAttackMove_;	// 攻撃の動きをするか
	bool isClicked_;	// クリックされているか

	// -------------------------------------------------
	// ↓ Animationに関する変数
	// -------------------------------------------------
	float animationTime_;
	float animationTransitionTime_;

	std::string nowAnimatonName_;// 今のanimationの名前
	std::string waitAnimationName_;// 待機時のanimationの名前

};
