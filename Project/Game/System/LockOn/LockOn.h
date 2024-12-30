#pragma once
// c++
#include <memory>
// engine
#include "Engine/Assets/GameObject/BaseGameObject.h"
// game
#include "Game/Manager/EnemyManager.h"

/// <summary>
/// Z注視するクラス
/// </summary>
class LockOn
	: public BaseGameObject {
public:

	LockOn();
	~LockOn();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void LockOnTarget();

public:

	void SetCameraMat(const Matrix4x4& cameraMat);

	void SetEnemyManger(EnemyManager* enmeyManager) { enemyManager_ = enmeyManager; }

private:

	void ChangeLockOnTarget();

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	const bool GetIsLockOn() const { return isLockOn_; }

	BaseGameObject* GetTarget() { return target_.lock().get(); }

private:

	EnemyManager* enemyManager_ = nullptr;

	// カメラの行列
	Matrix4x4 cameraMat_;

	std::weak_ptr<BaseGameObject> target_;

	bool isLockOn_;

	uint32_t lockOnEnemyIndex_ = 0;

};

