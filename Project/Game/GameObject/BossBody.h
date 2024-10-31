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

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "BossBody";

};

