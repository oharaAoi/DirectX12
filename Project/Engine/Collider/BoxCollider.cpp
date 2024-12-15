#include "BoxCollider.h"
#include <assert.h>

BoxCollider::BoxCollider() {}
BoxCollider::~BoxCollider() {}

void BoxCollider::Init(const std::string& tag, ColliderShape shape) {
	tag_ = tag;
	collisionState_ = CollisionFlags::NONE;

	if (shape == ColliderShape::AABB) {
		shape_ = AABB{ .min = Vector3::ZERO() * -1.0f, .max = Vector3::ZERO() };
	} else if(shape == ColliderShape::OBB) {
		shape_ = OBB{ .center = Vector3::ZERO(), .size = Vector3::ZERO() };
	} else {
		assert("not AABB or OBB Shape");
	}
}

void BoxCollider::Update() {
}

void BoxCollider::Draw() const {
}
