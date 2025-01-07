#pragma once
#include <stack>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/Assets/GameObject/Model.h"

class Trail {
public:

	struct Rect {
		Vector3 leftTop;
		Vector3 rightTop;
		Vector3 leftBottom;
		Vector3 rightBottom;
	};

	struct VertexData {
		Vector4 pos;
		Vector2 texcoord;
		Vector4 color;
	};

	struct MaterialData {
		Matrix4x4 uvTransform;
	};

	struct QuadData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
	};

public:

	Trail();
	~Trail();

	void Init();
	void Update();
	void Draw(const ViewProjection* viewprojection) const;

	void AddTrail(const Matrix4x4& mat);

	void ResetTrail();

private:

	static const uint32_t kMaxTrailNum_ = 500;
	uint32_t nowTrailCount_;

	std::vector<VertexData> verticesData_;
	std::vector<uint32_t> indices_;

	std::vector<QuadData> quades;

	// ------------------- DirectX関連 ------------------- //
	// VertexBuffer
	ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	VertexData* vertexData_ = nullptr;
	// IndexBuffer
	ComPtr<ID3D12Resource> indexBuffer_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	uint32_t* indexData_ = nullptr;

	uint32_t indexNum_;
	UINT vbvSize_;
	uint32_t vertexDataSize_;

	// material
	ComPtr<ID3D12Resource> materialBuffer_;
	MaterialData* material_;

	// transform
	std::unique_ptr<WorldTransform> transform_ = nullptr;

	// ------------------- Trail関連 ------------------- //

	float weight_ = 2.0f;

	uint32_t freeList_[kMaxTrailNum_];

	float time_ = 0.0f;
	float popTime_ = 0.0f;

	Rect rect_;

};

