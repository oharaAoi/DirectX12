#pragma once
#include <unordered_map>
#include "Game/Interface/ICharactorState.h"

class Player;

enum AttackStep {
	Step_NONE,
	Step_FIRST,
	Step_SECOND,
	Step_THEARD,
	Step_FOUR,

	Step_JUMPATTACK,
};

class PlayerAttackState :
    public ICharactorState {
public:

	struct Work {
		float time;
		float timeLimit;
	};

	struct AttackColliderActiveTime : public IJsonConverter {
		float activeStartTime[4];

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("first", activeStartTime[0])
				.Add("second", activeStartTime[1])
				.Add("therad", activeStartTime[2])
				.Add("four", activeStartTime[3])
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "first", activeStartTime[0]);
			fromJson(jsonData, "second", activeStartTime[1]);
			fromJson(jsonData, "therad", activeStartTime[2]);
			fromJson(jsonData, "four", activeStartTime[3]);
		}
	};

	using AttackPointer = void(PlayerAttackState::*)();

	std::unordered_map<AttackStep, AttackPointer> functionMap_ = {
		{AttackStep::Step_FIRST,	&PlayerAttackState::FirstAttack},
		{AttackStep::Step_SECOND,	&PlayerAttackState::SecondAttack},
		{AttackStep::Step_THEARD,	&PlayerAttackState::TheradAttack},
		{AttackStep::Step_FOUR,		&PlayerAttackState::FourAttack},
	};

public:
	PlayerAttackState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerAttackState();

	void Init() override;
	void Update() override;

	void CombAttack();

	void CheckNextAttack();

	void Attack(AttackStep step);

	void FirstAttack();

	void SecondAttack();

	void TheradAttack();

	void FourAttack();

	void JumpAttack();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Work work_;
	AttackColliderActiveTime colliderActiveTimes_;
	float colliderActiveTime_;

	Vector3 velocity_;
	Vector3 acceleration_;

	bool isComb_;

	uint32_t combNum_;
};

