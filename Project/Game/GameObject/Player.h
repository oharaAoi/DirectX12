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

	void SetReticlePos(const Vector3& reticlePos) { reticlrPos_ = reticlePos; }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

	static uint32_t GetScore() { return score_; }
	static void AddScore(uint32_t add) { score_ += add; }

	const bool GetIsShot() const { return isShot_; }
	void SetIsShot(bool isShot) { isShot_ = isShot; }

	const Quaternion GetShotQuaternion() const { return shotQuaternion_; }

private:

	// ポインタ
	GameScene* pGameScene_;
	// Reticleの座標
	Vector3 reticlrPos_;

	std::unique_ptr<BaseGameObject> test_;

	Quaternion shotQuaternion_;

	static uint32_t score_;

	bool isShot_;
	float shotEnergy_;
};

