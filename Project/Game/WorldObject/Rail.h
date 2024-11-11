#pragma once
#include "Engine/Engine.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/Mesh.h"
#include <unordered_map>
#include <memory>

class Rail  {
public:

	struct ForGPUData {
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 worldInverseTranspose;
		Vector4 color;
	};

public:

	Rail();
	~Rail();

	void Finalize() ;
	void Init(const std::string& directorPath, const std::string& fileName, const uint32_t& instanceNum);
	void Update(const Vector3& translate , const Quaternion& rotate, const Matrix4x4& view, const Matrix4x4& projection, uint32_t index) ;
	void Draw();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	
private:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;

	ForGPUData* forGpuData_;
	ComPtr<ID3D12Resource> forGpuDataBuffer_;

	DescriptorHeap::DescriptorHandles instancingSrvHandle_;

	uint32_t kNumInstance_;

};

