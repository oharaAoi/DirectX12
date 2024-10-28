#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class PlayerBullet : BaseGameObject {
public:

	PlayerBullet();
	~PlayerBullet();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetPopPos(const Vector3& pos);

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	const bool GetIsAlive() const { return isAlive_; }

private:

	Vector3 velocity_;

	float speed_;
	
	bool isAlive_;

};

