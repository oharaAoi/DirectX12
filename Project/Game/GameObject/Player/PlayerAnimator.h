#pragma once

class Player;

/// <summary>
/// PlayerのAnimationを管理するクラス
/// </summary>
class PlayerAnimator {
public:

	PlayerAnimator(Player* player) : pPlayer_(player) { Init(); };
	~PlayerAnimator();

	void Init();
	void Update();

private:

	Player* pPlayer_ = nullptr;

};

