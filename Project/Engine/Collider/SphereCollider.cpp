#include "SphereCollider.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/Render.h"
#include <assert.h>

SphereCollider::SphereCollider() {}
SphereCollider::~SphereCollider() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereCollider::Init(const std::string& tag, ColliderShape shape) {
	tag_ = tag;
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

void SphereCollider::Update(const SRT& srt) {
	std::get<Sphere>(shape_).center = srt.translate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereCollider::Draw() const {
	if (collisionState_ == CollisionFlags::ENTER || collisionState_ == CollisionFlags::STAY) {
		DrawSphere(std::get<Sphere>(shape_).center, std::get<Sphere>(shape_).radius, Render::GetViewProjectionMat(), Vector4(1, 0, 0, 1));
	} else {
		DrawSphere(std::get<Sphere>(shape_).center, std::get<Sphere>(shape_).radius, Render::GetViewProjectionMat());
	}
}