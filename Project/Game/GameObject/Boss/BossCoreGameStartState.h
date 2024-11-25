#pragma once
#include "Engine/Math/Vector3.h"
#include "Game/GameObject/State/BaseObjectState.h"

class BossCore;

class BossCoreGameStartState :
	public BaseObjectState {
public:

	BossCoreGameStartState(BossCore* pCore) : pCore_(pCore) { Init(); };
	~BossCoreGameStartState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	std::string groupName_ = "BossCoreGameStartState";

	BossCore* pCore_ = nullptr;
	float moveTime_ = 0.0f;

	bool isMove_ = false;
	
	Vector3 defaultPosition_{};
	Vector3 middlePosition_{};
	Vector3 endPosition_{};
};

