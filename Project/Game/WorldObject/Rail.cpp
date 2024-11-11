#include "Rail.h"

Rail::Rail() {
}

Rail::~Rail() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Finalize() {
	materialArray_.clear();
	meshArray_.clear();
	forGpuData_ = nullptr;
	forGpuDataBuffer_.Reset();
	DescriptorHeap::AddFreeSrvList(instancingSrvHandle_.assignIndex_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Init(const std::string& directorPath, const std::string& fileName, const uint32_t& instanceNum) {
	materialArray_ = LoadMaterialData(directorPath, fileName, Engine::GetDevice());
	meshArray_ = LoadVertexData(directorPath, fileName, Engine::GetDevice());

	kNumInstance_ = instanceNum;

	forGpuDataBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(ForGPUData) * kNumInstance_);
	// データをマップ
	forGpuDataBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&forGpuData_));

	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = instanceNum;
	desc.Buffer.StructureByteStride = sizeof(ForGPUData);

	instancingSrvHandle_ = Engine::GetHeap()->AllocateSRV();

	Engine::GetDevice()->CreateShaderResourceView(forGpuDataBuffer_.Get(), &desc, instancingSrvHandle_.handleCPU);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Update(const Vector3& translate, const Quaternion& rotate, const Matrix4x4& view, const Matrix4x4& projection, uint32_t index) {
	Matrix4x4 worldMat = Vector3(1, 1, 1).MakeScaleMat() * rotate.MakeMatrix() * translate.MakeTranslateMat();
	forGpuData_[index].world = worldMat;
	forGpuData_[index].view = view;
	forGpuData_[index].projection = projection;
	forGpuData_[index].worldInverseTranspose = Inverse(worldMat).Transpose();
	forGpuData_[index].color = Vector4(1, 1, 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Draw() {
	
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		std::string useMaterial = meshArray_[oi]->GetUseMaterial();
		
		meshArray_[oi]->Draw(commandList);
		materialArray_[useMaterial]->Draw(commandList);
		commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandle_.handleGPU);

		std::string textureName = materialArray_[useMaterial]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), kNumInstance_, 0, 0, 0);
	}
}


#ifdef _DEBUG
void Rail::Debug_Gui() {

}
#endif