#include "Skinning.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/Mesh.h"

Skinning::Skinning() {}
Skinning::~Skinning() {}

void Skinning::Finalize() {
	paletteResource_.Reset();
	influenceResource_.Reset();
	DescriptorHeap::AddFreeSrvList(paletteSrvHandle_.assignIndex_);
}

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
	uint32_t vertices = (uint32_t)mesh->GetVerticesData().size();
	uint32_t jointSize = (uint32_t)skeleton->GetJointsSize();
	vertices_ = (uint32_t)mesh->GetVerticesData().size();

	// -------------------------------------------------
	// ↓ palette用のResourceを確保
	// -------------------------------------------------
	paletteResource_ = CreateBufferResource(device, sizeof(WellForGPU) * jointSize);
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette, jointSize };	// spanを使ってアクセス
	//paletteSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);
	paletteSrvHandle_ = heap->AllocateSRV();
	
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

	//// -------------------------------------------------
	//// ↓ palette用のResourceを確保
	//// -------------------------------------------------
	//paletteResource_ = CreateBufferResource(device, sizeof(WellForGPU) * jointSize);
	//WellForGPU* mappedPalette = nullptr;
	//paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	//mappedPalette_ = { mappedPalette, jointSize };	// spanを使ってアクセス
	//paletteSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);

	//// -------------------------------------------------
	//// ↓ palette用のSRVを作成
	//// -------------------------------------------------
	//D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	//desc.Format = DXGI_FORMAT_UNKNOWN;
	//desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//desc.Buffer.FirstElement = 0;
	//desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	//desc.Buffer.NumElements = UINT(jointSize);
	//desc.Buffer.StructureByteStride = sizeof(WellForGPU);
	//device->CreateShaderResourceView(paletteResource_.Get(), &desc, paletteSrvHandle_.handleCPU);

	//// ---------------------------------------------------------------------------------
	//// ↓ influence用のResourceを確保。頂点ごとにinfluence情報を追加できるように
	//// ---------------------------------------------------------------------------------
	//influenceResource_ = CreateBufferResource(device, sizeof(VertexInfluence) * vertices);
	//VertexInfluence* mappedInfluence = nullptr;
	//influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	//std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * vertices);
	//mappedInfluence_ = { mappedInfluence, vertices };
	//influenceSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);

	//desc.Format = DXGI_FORMAT_UNKNOWN;
	//desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//desc.Buffer.FirstElement = 0;
	//desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	//desc.Buffer.NumElements = UINT(jointSize);
	//desc.Buffer.StructureByteStride = sizeof(VertexInfluence);
	//device->CreateShaderResourceView(influenceResource_.Get(), &desc, influenceSrvHandle_.handleCPU);

	//// -------------------------------------------------
	//// ↓ influence用VBVを作成
	//// -------------------------------------------------
	//influenceBuffeView_.BufferLocation = influenceResource_->GetGPUVirtualAddress();
	//influenceBuffeView_.SizeInBytes = UINT(sizeof(VertexInfluence) * vertices);
	//influenceBuffeView_.StrideInBytes = sizeof(VertexInfluence);

	//// -------------------------------------------------------------
	//// ↓ InverseBindPoseMatを格納する場所を作成して、単位行列で埋める
	//// -------------------------------------------------------------
	//inverseBindPoseMatrices_.resize(jointSize);
	//std::generate(inverseBindPoseMatrices_.begin(), inverseBindPoseMatrices_.end(), MakeIdentity4x4);

	//// -------------------------------------------------
	//// ↓ skinningInformationを作成
	//// -------------------------------------------------
	//skinningInformationResource_ = CreateBufferResource(device, sizeof(SkinningInformation));
	//skinningInformation_ = nullptr;
	//skinningInformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&skinningInformation_));
	//skinningInformation_->numVertices = vertices;
	//skinningInformationSrvHandle_ = heap->GetDescriptorHandle(TYPE_SRV);

	////// -------------------------------------------------
	////// ↓ outputResourceを作成
	////// -------------------------------------------------
	//D3D12_RESOURCE_DESC outputResourceDesc = {};
	//outputResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//outputResourceDesc.Width = sizeof(Vertex) * vertices; // 頂点データ数分のサイズ
	//outputResourceDesc.Height = 1;
	//outputResourceDesc.DepthOrArraySize = 1;
	//outputResourceDesc.MipLevels = 1;
	//outputResourceDesc.SampleDesc.Count = 1;
	//outputResourceDesc.SampleDesc.Quality = 0;
	//outputResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	//outputResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//outputResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	//outputResource_ = CreateUAVResource(device, sizeof(Vertex) * vertices);
	//D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	//uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	//uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	//uavDesc.Buffer.FirstElement = 0;
	//uavDesc.Buffer.NumElements = vertices;
	//uavDesc.Buffer.CounterOffsetInBytes = 0;
	//uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	//uavDesc.Buffer.StructureByteStride = sizeof(Vertex);
	//outputHandle_ = heap->GetDescriptorHandle(TYPE_SRV);
	//device->CreateUnorderedAccessView(outputResource_.Get(), nullptr, &uavDesc, outputHandle_.handleCPU);

	//vertexData_ = nullptr;
	//// アドレスを取得
	//outputResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//std::memcpy(vertexData_, mesh->GetVertexData(), sizeof(Mesh::VertexData) * vertices);

	//D3D12_HEAP_PROPERTIES heapProperties = {};
	//heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // UAVバッファはGPU専用のヒープに配置

	//D3D12_HEAP_PROPERTIES readbackHeapProps = {};
	//readbackHeapProps.Type = D3D12_HEAP_TYPE_READBACK;

	//D3D12_RESOURCE_DESC readbackBufferDesc = outputResourceDesc; // 同じサイズでREADBACKバッファを作成
	//readbackBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE; // UAVは不要
	//device->CreateCommittedResource(
	//	&readbackHeapProps,
	//	D3D12_HEAP_FLAG_NONE,
	//	&readbackBufferDesc,
	//	D3D12_RESOURCE_STATE_COPY_DEST,
	//	nullptr,
	//	IID_PPV_ARGS(&readResource_));

	//// -------------------------------------------------
	//// ↓ intputResourceを作成
	//// -------------------------------------------------
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // 頂点データなのでフォーマットはUNKNOWN
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//srvDesc.Buffer.FirstElement = 0;
	//srvDesc.Buffer.NumElements = static_cast<UINT>(vertices_);  // 頂点の数
	//srvDesc.Buffer.StructureByteStride = sizeof(Mesh::VertexData);  // 頂点1つあたりのサイズ
	//srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	//inputHandle_ = heap->GetDescriptorHandle(TYPE_SRV);
	//device->CreateShaderResourceView(mesh->GetVertexBuffer(), &srvDesc, inputHandle_.handleCPU);

	//// -------------------------------------------------
	//// ↓ ModelのskinClusterの情報を解析
	//// -------------------------------------------------
	//for (const auto& jointWeight : skinClusterData) {
	//	auto it = skeleton->GetJointMap().find(jointWeight.first);	// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
	//	if (it == skeleton->GetJointMap().end()) {
	//		continue;
	//	}
	//	//(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
	//	inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
	//	for (const auto& vertexWeight : jointWeight.second.vertexWeight) {
	//		auto& currentInfluence = mappedInfluence_[vertexWeight.vertexIndex];
	//		for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
	//			if (currentInfluence.weights[index] == 0.0f) {
	//				currentInfluence.weights[index] = vertexWeight.weight;
	//				currentInfluence.jointIndices[index] = (*it).second;
	//				break;
	//			}
	//		}
	//	}
	//}
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

void Skinning::RunCs(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetComputeRootDescriptorTable(0, paletteSrvHandle_.handleGPU);
	commandList->SetComputeRootDescriptorTable(1, inputHandle_.handleGPU);
	commandList->SetComputeRootDescriptorTable(2, influenceSrvHandle_.handleGPU);
	commandList->SetComputeRootDescriptorTable(3, outputHandle_.handleGPU);
	commandList->SetComputeRootConstantBufferView(4, skinningInformationResource_->GetGPUVirtualAddress());
	commandList->Dispatch((UINT(vertices_) + 1023) / 1024, 1, 1);
}

void Skinning::EndCS(ID3D12GraphicsCommandList* commandList, Mesh* mesh) {
	TransitionResourceState(commandList, outputResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList->CopyResource(readResource_.Get(), outputResource_.Get());
	TransitionResourceState(commandList, outputResource_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	
	Mesh::VertexData* mappedData = nullptr;
	D3D12_RANGE readRange = { 0, vertices_ };  // 頂点データのサイズ
	HRESULT hr = readResource_->Map(0, &readRange, reinterpret_cast<void**>(&mappedData));
	if (SUCCEEDED(hr)) {
		// 取得した頂点データ
		Mesh::VertexData* outputVertices = reinterpret_cast<Mesh::VertexData*>(mappedData);

		// 4. 元の頂点バッファにデータをコピーする
		Mesh::VertexData* originalVertices = mesh->GetVertexData(); // 元の頂点データの取得
		std::memcpy(originalVertices, outputVertices, sizeof(Mesh::VertexData) * vertices_);

	}
}
