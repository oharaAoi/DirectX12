#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 基本的な敵
/// </summary>
class NormalEnemy : public BaseGameObject {
public:

	NormalEnemy();
	~NormalEnemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	void OnCollisionEnter([[maybe_unused]] ICollider& other);
	void OnCollisionStay([[maybe_unused]] ICollider& other);

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	const bool GetIsDead() const { return isDead_; }

private:

	bool isDead_ = false;

};

