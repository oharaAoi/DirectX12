#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

/// <summary>
/// Playerがジャンプする際の処理
/// </summary>
class PlayerJumpState :
    public ICharactorState {
public:

	struct Work : public IJsonConverter {
		float jumpHight;

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("jumpHight", jumpHight)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "jumpHight", jumpHight);
		}
	};

public:

	PlayerJumpState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerJumpState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	void Jump();

	void JumpAttack();

private:

	Player* pPlayer_ = nullptr;

	Work work_;

	Vector3 velocity_;
	Vector3 acceleration_;
};

