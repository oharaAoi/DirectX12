#include "Skinning.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/GameObject/Model.h"

Skinning::Skinning() {}
Skinning::~Skinning() {}

void Skinning::Update(Skeleton& skeleton) {
	for (size_t jointIndex = 0; jointIndex < skeleton.GetJoints().size(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices_.size());
		mappedPalette_[jointIndex].skeltonSpaceMatrix = inverseBindPoseMatrices_[jointIndex] = skeleton.GetJoints()[jointIndex].skeltonSpaceMat;
		mappedPalette_[jointIndex].skeltonSpaceInverseTransposeMat = Transpose(Inverse(mappedPalette_[jointIndex].skeltonSpaceMatrix));
	}
}

void Skinning::CreateSkinCluster(ID3D12Device* device, const Skeleton& skeleton, Mesh* mesh,
								 DescriptorHeap* heap, std::map<std::string, Skinning::JointWeightData>& skinClusterData) {

	paletteResource_ = CreateBufferResource(device, sizeof(WellForGPU) * skeleton.GetJointsSize());
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette, skeleton.GetJointsSize() };
	paletteSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);

	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = UINT(skeleton.GetJointsSize());
	desc.Buffer.StructureByteStride = sizeof(WellForGPU);
	device->CreateShaderResourceView(paletteResource_.Get(), &desc, paletteSrvHandle_.handleCPU);

	influenceResource_ = CreateBufferResource(device, sizeof(VertexInfluence) * mesh->GetVerticesSize());
	VertexInfluence* mappedInfluene = nullptr;
	influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluene));
	std::memset(mappedInfluene, 0, sizeof(VertexInfluence) * mesh->GetVerticesSize());

	influenceBuffeView_.BufferLocation = influenceResource_->GetGPUVirtualAddress();
	influenceBuffeView_.SizeInBytes = UINT(sizeof(VertexInfluence) * mesh->GetVerticesSize());
	influenceBuffeView_.StrideInBytes = sizeof(VertexInfluence);

	inverseBindPoseMatrices_.resize(skeleton.GetJointsSize());
	std::generate(inverseBindPoseMatrices_.begin(), inverseBindPoseMatrices_.end(), MakeIdentity4x4);

	// -------------------------------------------------
	// ↓ ModelのskinClusterの情報を解析
	// -------------------------------------------------
	for (const auto& jointWeight : skinClusterData) {
		auto it = skeleton.GetJointMap().find(jointWeight.first);	// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == skeleton.GetJointMap().end()) {
			continue;
		}
		//(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeight) {
			auto& currentInfluence = mappedInfluence_[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
				}
			}
		}

	}
}
