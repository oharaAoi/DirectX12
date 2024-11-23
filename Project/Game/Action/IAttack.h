#pragma once

/// <summary>
/// 攻撃の基底クラス
/// </summary>
class IAttack {
public:

	IAttack() {};
	virtual ~IAttack() = default;

	virtual void Init() = 0;

	virtual void Attack() = 0;

	void SetMoveTime(float time) { moveTime_ = time; }

#ifdef _DEBUG
	virtual void Debug_Gui() = 0;
#endif

protected:

	float moveTime_;

};

