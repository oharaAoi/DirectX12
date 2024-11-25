#pragma once
#include "Engine/Math/Vector3.h"
#include "Game/GameObject/State/BaseObjectState.h"

class Boss;

/// <summary>
/// 登場する状態
/// </summary>
class BossAppearState
	: public BaseObjectState {
public:

	BossAppearState(Boss* pBoss) : pBoss_(pBoss) { Init(); };
	~BossAppearState();

	void Init()override;
	void Update() override;

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	const std::string groupName_ = "BossAppearState";

	Boss* pBoss_ = nullptr;

	Vector3 startPos_{};
	Vector3 endPos_{};
	Vector3 movePos_{};

	int appearEasing_ = 1;

	float moveTime_ = 0.0f;
	float moveTimeLimit_ = 0.0f;

	bool isAppear_;
	bool isShine_;
};

