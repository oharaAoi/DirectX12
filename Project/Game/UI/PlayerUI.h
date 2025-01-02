#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Engine/Assets/2d/Sprite.h"
#include "Game/GameObject/Player/Player.h"

class PlayerUI {
public:

	PlayerUI();
	~PlayerUI();

	void Init();
	void Update();
	void Draw() const;

public:

	// playerのポインタを設定
	void SetPlayer(Player* player) { pPlayer_ = player; };

private:

	Player* pPlayer_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> uiMap_;

};

