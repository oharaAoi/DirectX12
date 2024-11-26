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

	void Debug_Draw();

#ifdef _DEBUG
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
};

