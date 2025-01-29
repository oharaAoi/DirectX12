#include "ICollider.h"
#include <assert.h>

void ICollider::SwitchCollision(ICollider* partner) {
	int state = 0b00;

	auto it = collisionPartnersMap_.find(partner);
	if (it != collisionPartnersMap_.end()) {
		state = it->second;
	} else {
		collisionPartnersMap_[partner] = 0b00;
	}

	switch (state) {
	case 0b00:	// 衝突していない
		collisionState_ = CollisionFlags::ENTER;	// NONE → ENTER
		break;
	case 0b01:	// 初衝突
		collisionState_ = CollisionFlags::STAY;		// ENTER → STAY
		break;
	case 0b10:	// 衝突しなくなった直後
		collisionState_ = CollisionFlags::NONE;		// EXIT → NONE
		break;
	case 0b11:	// 連続衝突時
		break;
	default:
		// 存在していない衝突判定
		assert("not registered CollisionState");
		break;
	}
}

void ICollider::OnCollision(ICollider& other) {
	switch (collisionState_) {
	case 0b00:	// 衝突していない
		break;
	case 0b01:	// 初衝突
		OnCollisionEnter(other);
		break;
	case 0b10:	// 衝突しなくなった直後
		OnCollisionExit(other);
		break;
	case 0b11:	// 連続衝突時
		OnCollisionStay(other);
		break;
	default:
		// 存在していない衝突判定
		assert("not registered CollisionState");
		break;
	}
}

void ICollider::OnCollisionEnter(ICollider& other) {
	if (onCollisionEnter_) {
		onCollisionEnter_(other);
	}
}

void ICollider::OnCollisionStay(ICollider& other) {
	if (onCollisionStay_) {
		onCollisionStay_(other);
	}
}

void ICollider::OnCollisionExit(ICollider& other) {
	if (onCollisionExit_) {
		onCollisionExit_(other);
	}
}
