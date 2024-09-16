#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>

#include "Engine/Utilities/Loader.h"
#include "Engine/ParticleSystem/ParticleForGPU.h"
#include "Engine/Manager/TextureManager.h"

class BaseParticle {
public:

	BaseParticle() = default;
	~BaseParticle() = default;

	virtual void Finalize();

	virtual void Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName, const uint32_t& instanceNum);

	virtual void Update(const Matrix4x4& world, const Matrix4x4& viewMat, const Matrix4x4& projection,const Vector4& color, const uint32_t& index);

	virtual void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& numInstance);

	void CreateSRV(ID3D12Device* device, DescriptorHeap* dxHeap, const uint32_t& instanceNum);

protected:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;
	// 
	std::unique_ptr<ParticleForGPU> particleForGPU_;

	uint32_t kNumInstance_;
};

