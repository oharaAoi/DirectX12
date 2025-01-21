#include "GpuParticles.h"
#include "Engine/Engine.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Utilities/Loader.h"
#include <System/Manager/MeshManager.h>
#include <System/Manager/ModelManager.h>

GpuParticles::GpuParticles() {}
GpuParticles::~GpuParticles() {}

void GpuParticles::Finalize() {
	perViewBuffer_.Reset();
	perFrameBuffer_.Reset();
	particleResource_->Finalize();
	freeListIndexResource_->Finalize();
	freeListResource_->Finalize();
	meshArray_.clear();
	materials_.clear();
}

void GpuParticles::Init(uint32_t instanceNum) {
	// ポインタの取得
	DescriptorHeap* dxHeap = Engine::GetDxHeap();
	ID3D12Device* dxDevice = Engine::GetDevice();
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();

	kInstanceNum_ = instanceNum;

	// gpuに送るResourceの作成
	particleResource_ = std::make_unique<ShaderResource>();
	freeListIndexResource_ = std::make_unique<ShaderResource>();
	freeListResource_ = std::make_unique<ShaderResource>();

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_HEAP_PROPERTIES defaultHeapProperties{};
	defaultHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// particle
	particleResource_->Init(dxDevice, dxHeap, CreateUavResourceDesc(sizeof(Particle) * kInstanceNum_),
							&defaultHeapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COMMON);

	// freeListIndex
	freeListIndexResource_->Init(dxDevice, dxHeap, CreateUavResourceDesc(sizeof(uint32_t) * kInstanceNum_),
								 &defaultHeapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COMMON);

	// freeList
	freeListResource_->Init(dxDevice, dxHeap, CreateUavResourceDesc(sizeof(uint32_t) * kInstanceNum_),
							&defaultHeapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COMMON);

	// 各UAV, SRV
	particleResource_->CreateUAV(CreateUavDesc(kInstanceNum_, sizeof(Particle)));
	particleResource_->CreateSRV(CreateSrvDesc(kInstanceNum_, sizeof(Particle)));

	freeListIndexResource_->CreateUAV(CreateUavDesc(kInstanceNum_, sizeof(uint32_t)));
	freeListIndexResource_->CreateSRV(CreateSrvDesc(kInstanceNum_, sizeof(uint32_t)));

	freeListResource_->CreateUAV(CreateUavDesc(kInstanceNum_, sizeof(uint32_t)));
	freeListResource_->CreateSRV(CreateSrvDesc(kInstanceNum_, sizeof(uint32_t)));


	perViewBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerView));
	perViewBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perView_));

	// ゲーム情報
	perView_->viewProjection = Matrix4x4::MakeUnit();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	// mesh・Material情報
	meshArray_ = MeshManager::GetInstance()->GetMeshes("sphere.obj");
	materialData_ = LoadMaterialData(ModelManager::GetModelPath("sphere.obj"), "sphere.obj");

	for (const auto& material : materialData_) {
		materials_.emplace_back(Engine::CreateMaterial(material.second));
	}

	// -------------------------------------------------
	// ↓ Particleの初期化をGPUで行う
	// -------------------------------------------------

	Engine::SetCsPipeline(CsPipelineType::GpuParticleInit);
	InitBindCmdList(commandList, 0);
	commandList->Dispatch((UINT)kInstanceNum_ / 1024, 1, 1);
}

void GpuParticles::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	Engine::SetCsPipeline(CsPipelineType::GpuParticleUpdate);
	commandList->SetComputeRootDescriptorTable(0, particleResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(1, freeListIndexResource_->GetUAV().handleGPU);
	commandList->SetComputeRootConstantBufferView(2, perFrameBuffer_->GetGPUVirtualAddress());
	commandList->Dispatch((UINT)kInstanceNum_ / 1024, 1, 1);

	// UAVの変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = particleResource_->GetResource();
	commandList->ResourceBarrier(1, &barrier);
}

void GpuParticles::Draw(ID3D12GraphicsCommandList* commandList){
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		commandList->IASetVertexBuffers(0, 1, &meshArray_[oi]->GetVBV());
		commandList->IASetIndexBuffer(&meshArray_[oi]->GetIBV());
		commandList->SetGraphicsRootConstantBufferView(0, materials_[oi]->GetBufferAdress());
		commandList->SetGraphicsRootDescriptorTable(1, particleResource_->GetSRV().handleGPU);
		commandList->SetGraphicsRootConstantBufferView(3, perViewBuffer_->GetGPUVirtualAddress());

		std::string textureName = materials_[oi]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), kInstanceNum_, 0, 0, 0);
	}
}

void GpuParticles::InitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, particleResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 1, freeListIndexResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 2, freeListResource_->GetUAV().handleGPU);
}

void GpuParticles::EmitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, particleResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 1, freeListIndexResource_->GetUAV().handleGPU);
}

void GpuParticles::SetViewProjection(const Matrix4x4& viewProjection) {
	perView_->viewProjection = viewProjection;
}
