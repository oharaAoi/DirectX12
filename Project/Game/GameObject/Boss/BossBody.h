#pragma once
#include <string>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// bossの胴体となるクラス
/// </summary>
class BossBody : public BaseGameObject {
public:

	BossBody();
	~BossBody();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

	void ChangeAnimation(const std::string& preAnime, const std::string& afterAnime);

	void NowToAfterAnimation(const std::string& afterAnime);

#ifdef _DEBUG
	void Debug_Draw();

	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	const bool GetIsDecrementHp() const { return isDecrementHp_; }
	void SetIsDecrementHp(bool isDecrement) { isDecrementHp_ = isDecrement; }

private:

	std::string groupName_ = "BossBody";

	Vector3 startPos_;

	Vector3 defalutPos_;

	bool isDecrementHp_;

	float animationTime_;
	float animationTransitionTime_ = 1.0f;

	std::string nowAnimatonName_;// 今のanimationの名前
	std::string preAnimatonName_;// 今のanimationの名前
	std::string waitAnimationName_;// 待機時のanimationの名前
};

