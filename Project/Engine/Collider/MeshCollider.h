#pragma once
#include <functional>
#include "Engine/Assets/Mesh.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/Assets/WorldTransform.h"

enum CollisionFlags {
	NONE = 0b00,
	ENTER = 0b01,
	EXIT = 0b10,
	STAY = 0b11,
};

class MeshCollider {
public:

	MeshCollider();
	~MeshCollider();

	void Init(Mesh* mesh);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="worldTransform">: 生成するOBBの元となるworldTransform</param>
	/// <param name="offset">: offset座標(指定しなかったらZEROが入っている)</param>
	void Update(const WorldTransform* worldTransform, const Vector3& offset = Vector3::ZERO());
	void Draw() const;

	bool CheckCollisionList(MeshCollider* other);

	void AddColliderList(MeshCollider* other) { collisionList_.push_back(other); }
	void DeleteColliderList(MeshCollider* other) { collisionList_.remove(other); }

	/// <summary>
	/// 状態の変更
	/// </summary>
	void SwitchCollision();

	/// <summary>
	/// 衝突時にコールバック関数を呼び出す
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(MeshCollider& other, int collisionFlag);

	/// <summary>
	/// 最初の衝突時に呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionEnter(MeshCollider& other);

	/// <summary>
	/// 衝突中に呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionStay(MeshCollider& other);

	/// <summary>
	/// 衝突しなくなったら呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionExit(MeshCollider& other);

public:

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionEnter(std::function<void(MeshCollider&)> callback) {
		onCollisionEnter_ = callback;
	}

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionStay(std::function<void(MeshCollider&)> callback) {
		onCollisionStay_ = callback;
	}

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionExit(std::function<void(MeshCollider&)> callback) {
		onCollisionExit_ = callback;
	}

public:

	// ------------ 半径 ------------ // 
	float GetRadius() const { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }

	// ------------ Hitしているか ------------ // 
	void SetIsHitting(const bool& hit) { isHitting_ = hit; }
	const bool GetIsHitting() const { return isHitting_; }

	// --------------- tagの取得 -------------- //
	void SetTag(const std::string& tag) { tag_ = tag; }
	const std::string GetTag() const { return tag_; }

	// --------------- stateの取得 -------------- //
	void SetCollisionState(int stateBit) { collisionState_ = stateBit; }
	const int GetCollisionState() const { return collisionState_; }

	// --------------- 当たり判定をとるか -------------- //
	const bool GetIsnotCheckCollision() const { return isnotCheckCollision_; }
	void SetIsnotCheckCollision(bool isCollision) { isnotCheckCollision_ = isCollision; }

	// --------------- 当たった相手のリスト -------------- //
	std::list<MeshCollider*> GetMeshColliderList() { return collisionList_; }

	const OBB& GetOBB() { return obb_; }

	const Vector3 GetObbCenter() const { return obb_.center; }

private:

	// OBBのサイズ
	Vector3 size_;
	Vector3 maxSize_;
	Vector3 minSize_;
	// 半径
	float radius_ = 1.0f;
	// 当たり判定用
	OBB obb_;
	// タグ
	std::string tag_;
	// 
	Vector4 color_;

	bool isHitting_;
	bool isnotCheckCollision_;// このフラグがtrueの場合は当たり判定を行わない

	int collisionState_;

	// 元となるmesh
	Mesh* mesh_;
	// 衝突相手のリスト
	std::list<MeshCollider*> collisionList_;

	// 衝突用のコールバック
	std::function<void(MeshCollider&)> onCollisionEnter_;
	std::function<void(MeshCollider&)> onCollisionStay_;
	std::function<void(MeshCollider&)> onCollisionExit_;
};