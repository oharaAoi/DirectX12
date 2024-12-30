#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/System/Manager/ModelManager.h"
#include "Engine/Engine.h"

class GpuParticle {
public:	// メンバ構造体

	struct Particle {
		Vector3 scale;		// 拡縮
		Vector3 translate;	// 座標
		Vector3 velocity;	// 速度
		float lifeTime;		// 生存時間
		float currentTime;	// 現在の時間
		Vector4 color;		// 色
	};

	struct PerView {
		Matrix4x4 viewProjection;
		Matrix4x4 billboardMat;
	};

	struct PerFrame {
		float time;
		float deltaTime;
	};

public:

	GpuParticle();
	~GpuParticle();

	void Init(const std::string& modelName, uint32_t instanceNum);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	void EmitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

public:		// アクセッサ

	void SetPerViewProjectionMat(const Matrix4x4& viewProjectionMat) { perView_->viewProjection = viewProjectionMat; }

private:

	uint32_t kInstanceNum_;

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;

	ComPtr<ID3D12Resource> particleBuffer_;
	DescriptorHeap::DescriptorHandles uav_;
	DescriptorHeap::DescriptorHandles srv_;

	ComPtr<ID3D12Resource> freeListIndex_;
	DescriptorHeap::DescriptorHandles freeListIndexUav_;
	DescriptorHeap::DescriptorHandles freeListIndexSrv_;

	ComPtr<ID3D12Resource> freeList_;
	DescriptorHeap::DescriptorHandles freeListUav_;
	DescriptorHeap::DescriptorHandles freeListSrv_;

	ComPtr<ID3D12Resource> perViewBuffer_;
	PerView* perView_;

	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

};

