#include "MeshCollider.h"

MeshCollider::MeshCollider() {}
MeshCollider::~MeshCollider() {}

void MeshCollider::Init(Mesh* mesh) {
	mesh_ = mesh;

	size_ = Vector3::ZERO();
	maxSize_ = { -9999.0f, -9999.0f, -9999.0f };
	minSize_ = { 9999.0f, 9999.0f, 9999.0f };
	// meshのVertexから各軸での最大の値を取り出す
	Mesh::VertexData* vertices = mesh_->GetOutputVertexData();
	for (uint32_t index = 0; index < mesh_->GetVertexSize(); ++index) {
		//Vector3 size = Transform(Vector3(vertices[index].pos.x, vertices[index].pos.y , vertices[index].pos.z), worldMat);
		Vector3 size = Vector3(vertices[index].pos.x, vertices[index].pos.y, vertices[index].pos.z);

		// -------------------------------------------------
		// ↓ 最小を取得
		// -------------------------------------------------
		if (size.x < minSize_.x) {
			minSize_.x = size.x;
		}

		if (size.y < minSize_.y) {
			minSize_.y = size.y;
		}

		if (size.z < minSize_.z) {
			minSize_.z = size.z;
		}

		// -------------------------------------------------
		// ↓ 最大を取得
		// -------------------------------------------------
		if (size.x > maxSize_.x) {
			maxSize_.x = size.x;
		}

		if (size.y > maxSize_.y) {
			maxSize_.y = size.y;
		}

		if (size.z > maxSize_.z) {
			maxSize_.z = size.z;
		}
	}

	// -------------------------------------------------
	// ↓ 最大値と最小の値から物体の半径を求める
	// -------------------------------------------------
	float xRadius = std::abs(maxSize_.x - minSize_.x) * 0.5f;
	float yRadius = std::abs(maxSize_.y - minSize_.y) * 0.5f;
	float zRadius = std::abs(maxSize_.z - minSize_.z) * 0.5f;

	size_ = { xRadius, yRadius, zRadius };
}

void MeshCollider::Update(const WorldTransform* worldTransform) {
	maxSize_ = { -9999.0f, -9999.0f, -9999.0f };
	minSize_ = { 9999.0f, 9999.0f, 9999.0f };

	// meshのVertexから各軸での最大の値を取り出す
	Mesh::VertexData* vertices = mesh_->GetOutputVertexData();
	for (uint32_t index = 0; index < mesh_->GetVertexSize(); ++index) {
		Vector3 size = Vector3(vertices[index].pos.x, vertices[index].pos.y , vertices[index].pos.z);

		// -------------------------------------------------
		// ↓ 最小を取得
		// -------------------------------------------------
		if (size.x < minSize_.x) {
			minSize_.x = size.x;
		}

		if (size.y < minSize_.y) {
			minSize_.y = size.y;
		}

		if (size.z < minSize_.z) {
			minSize_.z = size.z;
		}

		// -------------------------------------------------
		// ↓ 最大を取得
		// -------------------------------------------------
		if (size.x > maxSize_.x) {
			maxSize_.x = size.x;
		}

		if (size.y > maxSize_.y) {
			maxSize_.y = size.y;
		}

		if (size.z > maxSize_.z) {
			maxSize_.z = size.z;
		}
	}

	// -------------------------------------------------
	// ↓ 最大値と最小の値から物体の半径を求める
	// -------------------------------------------------
	float xRadius = std::abs(maxSize_.x - minSize_.x) * 0.5f;
	float yRadius = std::abs(maxSize_.y - minSize_.y) * 0.5f;
	float zRadius = std::abs(maxSize_.z - minSize_.z) * 0.5f;
	size_ = { xRadius, yRadius, zRadius };

	// -------------------------------------------------
	// ↓ OBBの更新
	// -------------------------------------------------
	obb_.size = size_;
	obb_.center = worldMat->GetTranslation();
	obb_.MakeOBBAxis(worldMat->GetQuaternion());
}

void MeshCollider::Draw(const Matrix4x4& vpMat) const {
	DrawOBB(obb_, vpMat, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}
