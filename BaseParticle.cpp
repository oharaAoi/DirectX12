#include "BaseParticle.h"

void BaseParticle::Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName) {
	std::string path = directorPath + "/" + fileName;
	materialArray_ = LoadMaterialData(directorPath, fileName, device);
	meshArray_ = LoadVertexData(path, device);

	transformation_ = std::make_unique<TransformationMatrix>();
	transformation_->Init(device, 10);
}

void BaseParticle::Update(const Matrix4x4& world, const Matrix4x4& viewMat, const Matrix4x4& projection, const uint32_t& index) {
	transformation_->Update(world, viewMat, projection, index);
}

void BaseParticle::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materialArray_[meshArray_[oi]->GetUseMaterial()]->Draw(commandList);
		transformation_->DrawSRV(commandList);

	
		std::string textureName = materialArray_[meshArray_[oi]->GetUseMaterial()]->GetMateriaData().textureFilePath;
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName);

		UINT size = meshArray_[oi]->GetVertexSize() / sizeof(Mesh::VertexData);

		commandList->DrawIndexedInstanced(size, 10, 0, 0, 0);
	}
}

void BaseParticle::CreateSRV(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize, const uint32_t& index) {
	transformation_->CreateSrv(device, srvHeap, srvDescriptorSize, index);
}
