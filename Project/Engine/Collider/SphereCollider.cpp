#include "SphereCollider.h"
#include <assert.h>

SphereCollider::SphereCollider() {}
SphereCollider::~SphereCollider() {}

void SphereCollider::Init(const std::string& tag, ColliderShape shape) {
	tag_ = tag;
	collisionState_ = CollisionFlags::NONE;

	if (shape == ColliderShape::SPHERE) {
		shape_ = Sphere{ .center = Vector3::ZERO(), .radius = 1.0f };
	} else {
		assert("not Sphere Shape");
	}
}

void SphereCollider::Update() {

}

void SphereCollider::Draw() const {
}
