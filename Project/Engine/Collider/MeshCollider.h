#pragma once
#include <functional>
#include "Engine/Assets/Mesh.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/Assets/WorldTransform.h"

class MeshCollider {
public:

	MeshCollider();
	~MeshCollider();

	void Init(Mesh* mesh);
	void Update(const WorldTransform* worldTransform, const Vector3& offset);
	void Draw() const;

	/// <summary>
	/// 衝突時にコールバック関数を呼び出す
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(MeshCollider& other);

	/// <summary>
	/// 衝突時のコールバックを設定
	/// </summary>
	/// <param name="callback"></param>
	void SetOnCollisionCallBack(std::function<void(MeshCollider&)> callback) {
		onCollision_ = callback;
	}

	// ------------ 半径 ------------ // 
	float GetRadius() const { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }

	// ------------ Hitしているか ------------ // 
	void SetIsHitting(const bool& hit) { isHitting_ = hit; }
	const bool GetIsHitting() const { return isHitting_; }

	// --------------- オブジェクトの属性取得 -------------- //
	int32_t GetObjectType() { return typeID_; }

	// --------------- tagの取得 -------------- //
	void SetTag(const std::string& tag) { tag_ = tag; }
	const std::string GetTag() const { return tag_; }

	const OBB& GetOBB() { return obb_; }

	const Vector3 GetObbCenter() const { return obb_.center; }

private:

	// OBBのサイズ
	Vector3 size_;
	Vector3 maxSize_;
	Vector3 minSize_;

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

	// 元となるmesh
	Mesh* mesh_;

	// 衝突用のコールバック
	std::function<void(MeshCollider&)> onCollision_;
};