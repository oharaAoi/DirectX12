#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/2d/Sprite.h"

class PlayerUI {
public:

	PlayerUI();
	~PlayerUI();
	
	void Init();
	void Update();
	void Draw(bool canAttackBoss);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	// playerのscreen座標を設定
	void SetPlayerScreenPos(const Matrix4x4& playerMat, const Matrix4x4& vpvpMat);

private:

	std::unordered_map<std::string, std::unique_ptr<Sprite>> sprites_;

	Vector2 playerScreenPos_;

};

