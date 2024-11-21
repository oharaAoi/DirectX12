#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"
#include <functional>

class Player;

enum class BeAttackedType {
	NORMAL_HITED,
	SLAP_ATTACKED,
};


/// <summary>
/// Playerが攻撃を受けた際の状態
/// </summary>
class PlayerBeAttackedState :
	public BaseObjectState {

public:

	struct Work {
		float knockBackTime;
		float knockBackTimeLimit;
	};

public:

	PlayerBeAttackedState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerBeAttackedState();

	void Init() override;
	void Update() override;

	/// <summary>
	/// 時間の更新
	/// </summary>
	void TimeUpdate();

	void Reaction();

	/// <summary>
	/// 普通にぶつかった時
	/// </summary>
	void NormalKnockBack();

	/// <summary>
	/// 叩き潰された時
	/// </summary>
	void SlapedKnockBack();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	std::function<void()> knockBackFunction_;

	Player* pPlayer_ = nullptr;

	Work work_;

	BeAttackedType attackedType_;

	Vector3 knockBackVelocity_;
	float knockBackSpeed_;

};

