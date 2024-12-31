#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <span>
#include <map>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/Assets/Rigging/SkinCluster.h"

const uint32_t kNumMaxInfluence = 4;

class Skeleton;
class Model;
class Mesh;

/// <summary>
/// skinningの情報を管理しているクラス
/// </summary>
class Skinning {
public:

	/// <summary>
	/// 頂点に対して影響を与えるパラメータ群
	/// </summary>
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<uint32_t, kNumMaxInfluence> jointIndices;
	};

	struct WellForGPU {
		Matrix4x4 skeltonSpaceMatrix;
		Matrix4x4 skeltonSpaceInverseTransposeMat;
	};

	struct SkinningInformation {
		uint32_t numVertices;
	};

public:

	Skinning();
	~Skinning();

	void Finalize();

	void Update(Skeleton* skeleton);

	void CreateSkinCluster(ID3D12Device* device, Skeleton* skeleton, Mesh* mesh, DescriptorHeap* heap, const std::map<std::string, JointWeightData>& skinClusterData);

	void RunCs(ID3D12GraphicsCommandList* commandList) const;

	void EndCS(ID3D12GraphicsCommandList* commandList, Mesh* mesh);

	const D3D12_VERTEX_BUFFER_VIEW& GetVBV() { return vertexBufferView_; }

private:
	UINT vertices_;

	Mesh* mehs_;

	// palette
	ComPtr<ID3D12Resource> paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	DescriptorHeap::DescriptorHandles paletteSrvHandle_;

	// influence
	ComPtr<ID3D12Resource> influenceResource_;
	D3D12_VERTEX_BUFFER_VIEW influenceBuffeView_;
	std::span<VertexInfluence> mappedInfluence_;
	DescriptorHeap::DescriptorHandles influenceSrvHandle_;

	// well
	std::vector<Matrix4x4> inverseBindPoseMatrices_;

	// skinningInformation
	SkinningInformation* skinningInformation_;
	ComPtr<ID3D12Resource> skinningInformationResource_;
	// output
	ComPtr<ID3D12Resource> outputResource_;
	DescriptorHeap::DescriptorHandles outputHandle_;
	// MeshInput
	DescriptorHeap::DescriptorHandles inputHandle_;

	// vertex
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	// copy
	ComPtr<ID3D12Resource> copyResource_;

};

