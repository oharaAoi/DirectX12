#pragma once
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/Lib/Math/MyMath.h"

class Mesh final {
public: // 構造体

	struct VertexData {
		Vector4 pos;
		Vector2 texcoord;
		Vector3 normal;
		Vector4 worldPos;
		Vector3 tangent;
	};

	struct Vertices {
		Vector4 vertex1;// 左下
		Vector4 vertex2;// 上
		Vector4 vertex3;// 右下
	};

	struct RectVetices {
		Vector4 leftTop;
		Vector4 rightTop;
		Vector4 leftBottom;
		Vector4 rightBottom;
	};


public:

	Mesh();
	~Mesh();

	void Init(ID3D12Device* device, std::vector<VertexData> vertexData, std::vector<uint32_t>& indices);
	void Draw(ID3D12GraphicsCommandList* commandList);
	void Finalize();

	void DrawIndex(ID3D12GraphicsCommandList* commandList);

	void SetInitVertex();

public:

	const D3D12_VERTEX_BUFFER_VIEW& GetVBV() { return vertexBufferView_; }
	void SetVBV(const D3D12_VERTEX_BUFFER_VIEW& view) { vertexBufferView_ = view; }

	const D3D12_INDEX_BUFFER_VIEW& GetIBV() { return indexBufferView_; }

	/// <summary>
	/// vertexBuffer
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetVertexBuffer() { return vertexBuffer_.Get(); }

	/// <summary>
	/// vertexDataを取得する
	/// </summary>
	/// <returns></returns>
	VertexData* GetOutputVertexData() { return outputVertexData_; }

	/// <summary>
	/// indexの数を
	/// </summary>
	/// <returns></returns>
	uint32_t GetIndexNum() { return indexNum_; }

	/// <summary>
	/// indexBufferを取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetIndexBuffer() { return indexBuffer_.Get(); }

	/// <summary>
	/// indexDataを取得
	/// </summary>
	/// <returns></returns>
	uint32_t* GetIndexData() { return indexData_; }

	/// <summary>
	/// 配列に入っている頂点データ(生成時のデータ)
	/// </summary>
	/// <returns></returns>
	const std::vector<VertexData>& GetVerticesData() const { return verticesData_; }

	void SetOutputVertexData(VertexData* vertexData);

	/// <summary>
	/// 頂点数を取得する
	/// </summary>
	/// <returns></returns>
	const uint32_t GetVertexSize() const { return (uint32_t)verticesData_.size(); }

	void SetUseMaterial(const std::string& usemtl) { useMaterial_ = usemtl; }
	const std::string GetUseMaterial() const { return useMaterial_; }

private:
	// VertexBuffer
	ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	VertexData* vertexData_ = nullptr;
	VertexData* initVertexData_ = nullptr;
	VertexData* outputVertexData_ = nullptr;
	// IndexBuffer
	ComPtr<ID3D12Resource> indexBuffer_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	uint32_t* indexData_ = nullptr;

	std::vector<uint32_t> indices_;
	std::vector<VertexData> verticesData_;

	// 使用するマテリアル
	std::string useMaterial_;
	
	uint32_t indexNum_;
	UINT vbvSize_;
	uint32_t vertexDataSize_;
};