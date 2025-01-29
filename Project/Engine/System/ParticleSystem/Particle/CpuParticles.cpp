#include "CpuParticles.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Utilities/Loader.h"
#include <System/Manager/MeshManager.h>
#include <System/Manager/ModelManager.h>

CpuParticles::CpuParticles() {}
CpuParticles::~CpuParticles() { Finalize(); }

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void CpuParticles::Finalize() {
	meshArray_.clear();
	materials_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuParticles::Init() {
	meshArray_ = MeshManager::GetInstance()->GetMeshes("sphere.obj");
	materialData_ = LoadMaterialData(ModelManager::GetModelPath("sphere.obj"), "sphere.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuParticles::Update() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuParticles::Draw(ID3D12GraphicsCommandList* commandList) {
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		commandList->IASetVertexBuffers(0, 1, &meshArray_[oi]->GetVBV());
		commandList->IASetIndexBuffer(&meshArray_[oi]->GetIBV());
		commandList->SetGraphicsRootConstantBufferView(0, materials_[oi]->GetBufferAdress());
		//commandList->SetGraphicsRootDescriptorTable(1, particleResource_->GetSRV().handleGPU);
		
		std::string textureName = materials_[oi]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		//commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), kInstanceNum_, 0, 0, 0);
	}
}
