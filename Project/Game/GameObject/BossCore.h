#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

/// <summary>
/// Bossの攻撃をする本体
/// </summary>
class BossCore : public BaseGameObject {
public:

	BossCore();
	~BossCore();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

private:

	std::string groupName_ = "BossCore";

};
