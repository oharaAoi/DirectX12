#include "BaseParticle.h"

void BaseParticle::Finalize() {
	materialArray_.clear();
	meshArray_.clear();
	particleForGPU_->Finalize();
}

void BaseParticle::Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName, const uint32_t& instanceNum) {
	std::string path = directorPath + "/" + fileName;
	materialArray_ = LoadMaterialData(directorPath, fileName, device);
	meshArray_ = LoadVertexData(path, device);

	kNumInstance_ = instanceNum;

	particleForGPU_ = std::make_unique<ParticleForGPU>();
	particleForGPU_->Init(device, kNumInstance_);
}

void BaseParticle::Update(const Matrix4x4& world, const Matrix4x4& viewMat, const Matrix4x4& projection, const Vector4& color, const uint32_t& index) {
	particleForGPU_->Update(world, viewMat, projection, color, index);
}

void BaseParticle::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& numInstance) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materialArray_[meshArray_[oi]->GetUseMaterial()]->Draw(commandList);
		particleForGPU_->StackCommand(commandList);

		std::string textureName = materialArray_[meshArray_[oi]->GetUseMaterial()]->GetMateriaData().textureFilePath;
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		UINT size = meshArray_[oi]->GetVertexSize() / sizeof(Mesh::VertexData);

		commandList->DrawIndexedInstanced(size, numInstance, 0, 0, 0);
	}
}

void BaseParticle::CreateSRV(ID3D12Device* device, DescriptorHeap* dxHeap, const uint32_t& instanceNum) {
	particleForGPU_->CreateSrv(device, dxHeap, instanceNum);
}
