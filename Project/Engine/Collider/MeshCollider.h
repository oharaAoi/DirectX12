#pragma once
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

private:

	// OBBのサイズ
	Vector3 size_;

	Vector3 maxSize_;
	Vector3 minSize_;

	OBB obb_;

	// 元となるmesh
	Mesh* mesh_;

};

