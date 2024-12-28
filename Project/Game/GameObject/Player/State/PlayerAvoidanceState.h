#pragma once
#include "Game/Interface/ICharactorState.h"
#include "Engine/Math/Vector3.h"

class Player;

/// <summary>
/// 回避状態
/// </summary>
class PlayerAvoidanceState :
    public ICharactorState {
public:

	struct Work : public IJsonConverter {
		float time;
		float timeLimit;

		float avoidanceLength;	// 回避を行う距離

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("timeLimit", timeLimit)
				.Add("avoidanceLength", avoidanceLength)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "timeLimit", timeLimit);
			fromJson(jsonData, "avoidanceLength", avoidanceLength);
		}
	};

public:

	PlayerAvoidanceState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerAvoidanceState();

	void Init() override;
	void Update() override;

	/// <summary>
	/// Playerを移動させる
	/// </summary>
	void RollMove();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Work work_;
	Player* pPlayer_ = nullptr;

	Vector3 velocity_;

};

