#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

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

	void Load();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	const std::string kDirectoryPath = "./Game/Resources/GameData/PlayerAnimations/";

	Player* pPlayer_ = nullptr;

	std::vector<std::string> playerAnimationNames_;

#ifdef _DEBUG
	std::string selectName_;
#endif

};

