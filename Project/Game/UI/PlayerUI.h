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

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	// playerのポインタを設定
	void SetPlayer(Player* player) { pPlayer_ = player; };

private:

	Player* pPlayer_ = nullptr;

	static const uint32_t kMaxPlayerHp_ = 5;

	std::unique_ptr<Sprite> hpes_[kMaxPlayerHp_];

	Vector2 firstHpPos_;
	float interval_;

};

