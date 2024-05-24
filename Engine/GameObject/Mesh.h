#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
// DirectX
#include "DirectXUtils.h"
// math
#include "MyMath.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class Mesh final {
public: // 構造体

	struct VertexData {
		Vector4 pos;
		Vector2 texcoord;
		Vector3 normal;
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

	void Init(ID3D12Device* device, const uint32_t& vBSize, const uint32_t& iBSize);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

public:
	/// <summary>
	/// vertexBuffer
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetVertexBuffer() { return vertexBuffer_.Get(); }

	/// <summary>
	/// vertexDataを取得する
	/// </summary>
	/// <returns></returns>
	VertexData* GetVertexData() { return vertexData_; }

	void SetVertexData(std::vector<VertexData> vertexData);

	UINT GetVertexSize() { return vertexBufferSize_; }

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

	void SetUseMaterial(const std::string& usemtl) { useMaterial_ = usemtl; }
	std::string GetUseMaterial() const { return useMaterial_; }

private:
	// VertexBuffer
	Comptr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	VertexData* vertexData_ = nullptr;
	// IndexBuffer
	Comptr<ID3D12Resource> indexBuffer_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	uint32_t* indexData_ = nullptr;

	// 使用するマテリアル
	std::string useMaterial_;

	uint32_t vertexBufferSize_;
};