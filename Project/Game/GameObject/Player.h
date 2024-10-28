#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class GameScene;

class Player
	: public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void Shot();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetPGameScene(GameScene* gameScene) { pGameScene_ = gameScene; }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }

private:

	// ポインタ
	GameScene* pGameScene_;
};

