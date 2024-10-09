#include "Skinning.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/GameObject/Model.h"

Skinning::Skinning() {}
Skinning::~Skinning() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skinning::Update(Skeleton* skeleton) {
	for (size_t jointIndex = 0; jointIndex < skeleton->GetJoints().size(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices_.size());

		// jointのlocalからskeletonの空間への変換行列
		mappedPalette_[jointIndex].skeltonSpaceMatrix =
			inverseBindPoseMatrices_[jointIndex] * skeleton->GetJoints()[jointIndex].skeltonSpaceMat;

		// 変換行列のInverse
		mappedPalette_[jointIndex].skeltonSpaceInverseTransposeMat =
			Transpose((mappedPalette_[jointIndex].skeltonSpaceMatrix).Inverse());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CPUで作られたデータをGPUで扱えるように
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skinning::CreateSkinCluster(ID3D12Device* device, Skeleton* skeleton, Mesh* mesh, DescriptorHeap* heap, std::map<std::string, Skinning::JointWeightData>& skinClusterData) {
	uint32_t vertices = (uint32_t)mesh->GetIndexNum();
	uint32_t jointSize = (uint32_t)skeleton->GetJointsSize();

	// -------------------------------------------------
	// ↓ palette用のResourceを確保
	// -------------------------------------------------
	paletteResource_ = CreateBufferResource(device, sizeof(WellForGPU) * jointSize);
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette, jointSize };	// spanを使ってアクセス
	paletteSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);

	// -------------------------------------------------
	// ↓ palette用のSRVを作成
	// -------------------------------------------------
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = UINT(jointSize);
	desc.Buffer.StructureByteStride = sizeof(WellForGPU);
	device->CreateShaderResourceView(paletteResource_.Get(), &desc, paletteSrvHandle_.handleCPU);

	// ---------------------------------------------------------------------------------
	// ↓ influence用のResourceを確保。頂点ごとにinfluence情報を追加できるように
	// ---------------------------------------------------------------------------------
	influenceResource_ = CreateBufferResource(device, sizeof(VertexInfluence) * vertices);
	VertexInfluence* mappedInfluence = nullptr;
	influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * vertices);
	mappedInfluence_ = { mappedInfluence, vertices };

	// -------------------------------------------------
	// ↓ influence用VBVを作成
	// -------------------------------------------------
	influenceBuffeView_.BufferLocation = influenceResource_->GetGPUVirtualAddress();
	influenceBuffeView_.SizeInBytes = UINT(sizeof(VertexInfluence) * vertices);
	influenceBuffeView_.StrideInBytes = sizeof(VertexInfluence);

	// -------------------------------------------------------------
	// ↓ InverseBindPoseMatを格納する場所を作成して、単位行列で埋める
	// -------------------------------------------------------------
	inverseBindPoseMatrices_.resize(jointSize);
	std::generate(inverseBindPoseMatrices_.begin(), inverseBindPoseMatrices_.end(), MakeIdentity4x4);

	// -------------------------------------------------
	// ↓ ModelのskinClusterの情報を解析
	// -------------------------------------------------
	for (const auto& jointWeight : skinClusterData) {
		auto it = skeleton->GetJointMap().find(jointWeight.first);	// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == skeleton->GetJointMap().end()) {
			continue;
		}
		//(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeight) {
			auto& currentInfluence = mappedInfluence_[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画コマンドを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skinning::StackCommand(ID3D12GraphicsCommandList* commandList, const D3D12_VERTEX_BUFFER_VIEW& vbv) const {
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		vbv,				// vertexData
		influenceBuffeView_	// ifluence
	};

	commandList->IASetVertexBuffers(0, 2, vbvs);
	commandList->SetGraphicsRootDescriptorTable(4, paletteSrvHandle_.handleGPU);
}
