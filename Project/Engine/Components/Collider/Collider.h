#pragma once
#include <cstdint>
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Components/Collider/MeshCollider.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	static bool isColliderBoxDraw_;

public:

	Collider();
	virtual ~Collider();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

public:

	// ------------ 位置(world) ------------ // 
	const virtual Vector3 GetWorldPos() const = 0;

	// ------------ 半径 ------------ // 
	float GetRadius() const { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }

	// ------------ Hitしているか ------------ // 
	void SetIsHitting(const bool& hit) { isHitting_ = hit; }
	const bool GetIsHitting() const { return isHitting_; }

	// --------------- オブジェクトの属性取得 -------------- //
	int32_t GetObjectType(){ return typeID_; }

	// --------------- tagの取得 -------------- //
	void SetTag(const std::string& tag) { tag_ = tag; }
	const std::string GetTag() const { return tag_; }

	OBB& GetObb() { return obb_; }
	void SetObbSize(float radius){ obb_.size = { radius,radius,radius }; }

protected:

	// 半径
	float radius_ = 1.0f;
	// 種別ID
	int32_t typeID_ = -1;
	// 当たり判定用
	OBB obb_;
	// タグ
	std::string tag_;

	bool isHitting_;
	Vector4 color_;

};

