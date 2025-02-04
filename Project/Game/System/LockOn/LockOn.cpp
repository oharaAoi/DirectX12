#include "LockOn.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/System/Input/Input.h"

LockOn::LockOn() {}
LockOn::~LockOn() {}
void LockOn::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LockOn::Init() {
	BaseGameObject::Init();
	SetObject("plane.obj");
	SetIsLighting(false);
	SetTexture("reticle.png");


	lockOnEnemyIndex_ = 0;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&LockOn::Debug_Gui, this), "lockOn");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LockOn::Update() {
	if (!target_.expired()) {
		ChangeLockOnTarget();
		transform_->SetTranslaion(target_.lock()->GetTransform()->GetTranslation());
	} else if (isLockOn_) {
		lockOnEnemyIndex_ = 0;

		if (enemyManager_->GetNormalEnemyList().size() != 0) {
			target_ = enemyManager_->GetNearEnemy(lockOnEnemyIndex_);
		} else {
			target_.reset();
			isLockOn_ = false;
		}
	}
	transform_->rotation_ = Quaternion::LookAt(Vector3(), cameraPos_);
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LockOn::Draw() const {
	if (isLockOn_) {
		BaseGameObject::Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　LockOnする処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LockOn::LockOnTarget() {
	if (enemyManager_->GetNormalEnemyList().size() == 0) {
		return;
	}
	isLockOn_ = !isLockOn_;

	if (isLockOn_) {
		lockOnEnemyIndex_ = 0;
		// 一番近い敵を探索する
		enemyManager_->CheckNearEnemyList();
		// 指定した敵のポインタを取得する
		target_ = enemyManager_->GetNearEnemy(lockOnEnemyIndex_);
	} else {
		target_.reset();
		isLockOn_ = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Targetを変更する処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LockOn::ChangeLockOnTarget() {
	if (Input::GetIsPadTrigger(XInputButtons::L_SHOULDER)) {
		if (enemyManager_->GetNormalEnemyList().size() != 0) {
			lockOnEnemyIndex_++;
			target_ = enemyManager_->GetNearEnemy(lockOnEnemyIndex_);
		}
	}
}

void LockOn::SetCameraMat(const Matrix4x4& cameraMat) {
	cameraMat_ = cameraMat;
	cameraMat_.m[3][0] = 0.0f;
	cameraMat_.m[3][1] = 0.0f;
	cameraMat_.m[3][2] = 0.0f;
	cameraMat_ = cameraMat_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void LockOn::Debug_Gui() {
	BaseGameObject::Debug_Gui();
}
#endif