#pragma once
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"

class Sprite {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct TextureTransformData {
		Matrix4x4 wvp;
	};

public:

	Sprite();
	~Sprite();

	void Init(ID3D12Device* device, const Mesh::RectVetices& rect);

	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> transformBuffer_;

	// view
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	// データ
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;
	TextureTransformData* transformData_;

	// Transform情報
	kTransform transform_;
	kTransform uvTransform_;
};