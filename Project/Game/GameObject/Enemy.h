#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"

class Enemy
	: public BaseGameObject , public Collider {
public:

	Enemy();
	~Enemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other) override;

#ifdef _DEBUG
	void DeBug_Gui() {};
#endif

	const Vector3 GetWorldPos() const override;

	const bool GetIsAlive() const { return isAlive_; }
	
private:

	bool isAlive_;

};

