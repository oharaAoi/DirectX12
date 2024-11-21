#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <Engine/Assets/Animetor.h>

class Player;

using json = nlohmann::json;

/// <summary>
/// PlayerのAnimationを管理するクラス
/// </summary>
class PlayerAnimator {
public:

	PlayerAnimator(Player* player) : pPlayer_(player) { Init(); };
	~PlayerAnimator();

	void Init();
	void Update();

	void Transition(const std::string& beforeState, const std::string afterState);
	void NowToAfterTransition(const std::string afterState);

	void Load();

#ifdef _DEBUG
	void Debug_Gui();

private:

	void Debug_CreateState();

	void Save();
#endif

public:

	const std::string& GetNowState() const { return nowState_; }

private:

	const std::string kDirectoryPath = "./Game/Resources/GameData/PlayerAnimations/";

	// playerのポインタ
	Player* pPlayer_ = nullptr;
	// playerのanimatorのポインタ
	Animetor* pPlayerAnimator_ = nullptr;

	// 現在の状態
	std::string nowState_;
	// 状態とanimationの名前をmapに保存
	std::unordered_map<std::string, std::string> stateAnimationMap_;

	float animtionTime_;

#ifdef _DEBUG
	std::string selectName_;

	std::string stateName_;

	bool popCreateWindow_;

	std::unordered_map<std::string, std::string> debug_stateAnimationMap_;
#endif

};

