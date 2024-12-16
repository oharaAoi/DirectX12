#include "BoxCollider.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/Render.h"
#include <assert.h>

BoxCollider::BoxCollider() {}
BoxCollider::~BoxCollider() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxCollider::Init(const std::string& tag, ColliderShape shape) {
	tag_ = tag;
	collisionState_ = CollisionFlags::NONE;
	
	if (shape == ColliderShape::AABB) {
		shape_ = AABB{ .min = Vector3::UNIT() * -1.0f, .max = Vector3::UNIT() };
	} else if(shape == ColliderShape::OBB) {
		shape_ = OBB{ .center = Vector3::ZERO(), .size = Vector3::UNIT() };
	} else {
		assert("not AABB or OBB Shape");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxCollider::Update(const SRT& srt) {
	if (std::holds_alternative<AABB>(shape_)) {
		std::get<AABB>(shape_).min = srt.translate - (size_ / 2.0f);
		std::get<AABB>(shape_).max = srt.translate + (size_ / 2.0f);
	} else if (std::holds_alternative<OBB>(shape_)) {
		std::get<OBB>(shape_).center = srt.translate;
		std::get<OBB>(shape_).MakeOBBAxis(srt.rotate);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BoxCollider::Draw() const {
	Vector4 color{ 1,1,1,1 };
	if (collisionState_ == CollisionFlags::ENTER || collisionState_ == CollisionFlags::STAY) {
		color = { 1,0,0,1 };
	}

	if (std::holds_alternative<AABB>(shape_)) {
		DrawAABB(std::get<AABB>(shape_), Render::GetViewProjectionMat(), color);
	} else if (std::holds_alternative<OBB>(shape_)) {
		DrawOBB(std::get<OBB>(shape_), Render::GetViewProjectionMat(), color);
	}
}
