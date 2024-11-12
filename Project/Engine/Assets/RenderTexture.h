#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/ImGuiManager.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class RenderTexture {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color;
		Matrix4x4 uvTransform;
		Vector2 uvDrawRange;		// 0~1の範囲で指定する
		float padding[2];
	};

	struct TextureTransformData {
		Matrix4x4 wvp;
	};

public:

	RenderTexture();
	~RenderTexture();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* dxHeap);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

	void TransitionResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetUAV() { return renderUavRenderAddress_.handleGPU; }

private:

	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> transformBuffer_;
	ComPtr<ID3D12Resource> renderResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	DescriptorHeap::DescriptorHandles renderUavRenderAddress_;
	DescriptorHeap::DescriptorHandles renderSrvRenderAddress_;
	
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;
	TextureTransformData* transformData_;
};

