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
	void Update(const WorldTransform* worldTransform);
	void Draw(const Matrix4x4& vpMat) const;

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

	const OBB& GetOBB() { return obb_; }
	
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

