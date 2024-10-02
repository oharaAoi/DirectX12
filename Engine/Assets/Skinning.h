#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <span>
#include <utility>
#include <map>
#include <algorithm>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/Assets/Mesh.h"

const uint32_t kNumMaxInfluence = 4;

class Skeleton;
class Model;

/// <summary>
/// skinningの情報を管理しているクラス
/// </summary>
class Skinning {
public:

	struct VertexWeightData {
		float weight;
		uint32_t vertexIndex;
	};

	struct JointWeightData {
		Matrix4x4 inverseBindPoseMatrix;
		std::vector<VertexWeightData> vertexWeight;
	};

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

public:

	Skinning();
	~Skinning();

	void Update(Skeleton* skeleton);

	void CreateSkinCluster(ID3D12Device* device, Skeleton* skeleton, Mesh* mesh, DescriptorHeap* heap, std::map<std::string, Skinning::JointWeightData>& skinClusterData);

	void StackCommand(ID3D12GraphicsCommandList* commandList, const D3D12_VERTEX_BUFFER_VIEW& vbv) const;

private:

	std::vector<Matrix4x4> inverseBindPoseMatrices_;

	ComPtr<ID3D12Resource> influenceResource_;
	D3D12_VERTEX_BUFFER_VIEW influenceBuffeView_;
	std::span<VertexInfluence> mappedInfluence_;

	ComPtr<ID3D12Resource> paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	DescriptorHeap::DescriptorHandles paletteSrvHandle_;

};

