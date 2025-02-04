#include "BaseCpuParticle.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Utilities/Loader.h"
#include "Engine/Render.h"
#include <System/Manager/MeshManager.h>
#include <System/Manager/ModelManager.h>

BaseCpuParticle::BaseCpuParticle() {
}

BaseCpuParticle::~BaseCpuParticle() {
}

void BaseCpuParticle::Finalize() {
	meshArray_.clear();
	materials_.clear();
	particleResource_.Reset();
	perViewBuffer_.Reset();
}

void BaseCpuParticle::Init(uint32_t instanceNum, const std::string& objFile) {
	DescriptorHeap* dxHeap = Engine::GetDxHeap();
	ID3D12Device* dxDevice = Engine::GetDevice();
	
	kInstanceNum_ = instanceNum;

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	
	particleResource_ = CreateBufferResource(dxDevice, sizeof(GpuData) * kInstanceNum_);
	srvAddress_ = dxHeap->AllocateSRV();

	D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSrvDesc(kInstanceNum_, sizeof(GpuData));
	dxDevice->CreateShaderResourceView(particleResource_.Get(), &desc, srvAddress_.handleCPU);

	perViewBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerView));
	perViewBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perView_));

	// ゲーム情報
	perView_->viewProjection = Matrix4x4::MakeUnit();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	meshArray_ = MeshManager::GetInstance()->GetMeshes(objFile);
	materialData_ = LoadMaterialData(ModelManager::GetModelPath(objFile), objFile);

	for (const auto& material : materialData_) {
		materials_.emplace_back(Engine::CreateMaterial(material.second));
	}

	for (auto& material : materials_) {
		material->SetColor({ 1, 1, 1, 1 });
	}

	particleResource_->Map(0, nullptr, reinterpret_cast<void**>(&particleData_));

	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		particleData_[oi].worldMat = Matrix4x4::MakeUnit();
		particleData_[oi].color = Vector4(0, 0, 0, 0);
	}

	particleArray_.resize(kInstanceNum_);
	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		particleArray_[oi].translate = {};
		particleArray_[oi].scale = { 1,1,1 };
		particleArray_[oi].rotate = {};

		particleArray_[oi].velocity = {};
		particleArray_[oi].acceleration = {};

		particleArray_[oi].lifeTime = 0.0f;
		particleArray_[oi].currentTime = 0.0f;
		particleArray_[oi].damping = 0.0f;
		particleArray_[oi].gravity = 0.0f;
	}
}

void BaseCpuParticle::Update() {
	perView_->viewProjection = Render::GetViewProjectionMat();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime >= 0.0f) {
			particleArray_[oi].lifeTime -= GameTimer::DeltaTime();

			particleArray_[oi].velocity += particleArray_[oi].acceleration * GameTimer::DeltaTime();
			particleArray_[oi].translate += particleArray_[oi].velocity;

			particleData_[oi].worldMat = Matrix4x4::MakeAffine(
				particleArray_[oi].scale, 
				particleArray_[oi].rotate,
				particleArray_[oi].translate);

			particleArray_[oi].color.w -= GameTimer::DeltaTime();

			particleData_[oi].color = particleArray_[oi].color;

		} else {
			particleData_[oi].worldMat = Matrix4x4::MakeUnit();
			particleData_[oi].color = Vector4(0, 0, 0, 0);
		}
	}
}

void BaseCpuParticle::Draw(ID3D12GraphicsCommandList* commandList) {
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		commandList->IASetVertexBuffers(0, 1, &meshArray_[oi]->GetVBV());
		commandList->IASetIndexBuffer(&meshArray_[oi]->GetIBV());
		commandList->SetGraphicsRootConstantBufferView(0, materials_[oi]->GetBufferAdress());
		commandList->SetGraphicsRootDescriptorTable(1, srvAddress_.handleGPU);

		std::string textureName = materials_[oi]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		commandList->SetGraphicsRootConstantBufferView(3, perViewBuffer_->GetGPUVirtualAddress());

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), kInstanceNum_, 0, 0, 0);
	}
}

