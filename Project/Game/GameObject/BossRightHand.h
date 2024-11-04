#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

/// <summary>
/// Bossの右手
/// </summary>
class BossRightHand : public BaseGameObject {
public:

	BossRightHand();
	~BossRightHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "BossRightHand";

	std::unique_ptr<MeshCollider> meshCollider_;
};

