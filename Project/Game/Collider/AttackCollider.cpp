#include "AttackCollider.h"
#include "Engine/Utilities/DrawUtils.h"
#include <assert.h>

AttackCollider::AttackCollider() {
}

AttackCollider::~AttackCollider() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AttackCollider::Init(uint32_t bitTag, ColliderShape shape) {
	bitTag_ = bitTag;
	collisionState_ = CollisionFlags::NONE;

	if (shape == ColliderShape::SPHERE) {
		shape_ = Sphere{ .center = Vector3::ZERO(), .radius = 1.0f };
	} else {
		assert("not Sphere Shape");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AttackCollider::Update(const QuaternionSRT& srt) {
	std::get<Sphere>(shape_).center = srt.translate;

	centerPos_ = srt.translate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AttackCollider::Draw() const {
	if (!isActive_) {
		return;
	}

	if (collisionState_ == CollisionFlags::ENTER || collisionState_ == CollisionFlags::STAY) {
		DrawSphere(std::get<Sphere>(shape_).center, std::get<Sphere>(shape_).radius, Render::GetViewProjectionMat(), Vector4(1, 0, 0, 1));
	} else {
		DrawSphere(std::get<Sphere>(shape_).center, std::get<Sphere>(shape_).radius, Render::GetViewProjectionMat());
	}
}
