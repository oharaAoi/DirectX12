#pragma once
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/System/Manager/TextureManager.h"
#include "Engine/System/Manager/ImGuiManager.h"
#include "Engine/DirectX/Resource/ShaderResource.h"

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
		Vector2 uvMinSize;		// 0~1の範囲で指定
		Vector2 uvMaxSize;		// 0~1の範囲で指定
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

#ifdef _DEBUG
	void DrawGui();
#endif // _DEBUG

	void TransitionResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetUAV() { return renderResource_->GetUAV().handleGPU; }

private:

	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> transformBuffer_;
	
	std::unique_ptr<ShaderResource> renderResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;
	TextureTransformData* transformData_;
};

