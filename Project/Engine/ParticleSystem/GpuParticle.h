#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Manager/ModelManager.h"
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

public:

	GpuParticle();
	~GpuParticle();

	void Init(const std::string& modelName, uint32_t instanceNum);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	uint32_t kInstanceNum_;

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;

	ComPtr<ID3D12Resource> particleBuffer_;
	DescriptorHeap::DescriptorHandles uav_;
	DescriptorHeap::DescriptorHandles srv_;

	ComPtr<ID3D12Resource> perViewBuffer_;
	PerView* perView_;
};

