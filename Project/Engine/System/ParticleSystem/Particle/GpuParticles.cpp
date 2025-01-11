#include "GpuParticles.h"
#include "Engine/Engine.h"

GpuParticles::GpuParticles() {
}

GpuParticles::~GpuParticles() {
	perViewBuffer_.Reset();
	perFrameBuffer_.Reset();
	particleResource_->Finalize();
	freeListIndexResource_->Finalize();
	freeListResource_->Finalize();
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
	particleResource_->Init(dxDevice, dxHeap, &CreateUploadResourceDesc(sizeof(Particle) * kInstanceNum_),
							&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

	// freeListIndex
	freeListIndexResource_->Init(dxDevice, dxHeap, &CreateUavResourceDesc(sizeof(uint32_t) * kInstanceNum_),
								 &defaultHeapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COMMON);

	// freeList
	freeListResource_->Init(dxDevice, dxHeap, &CreateUavResourceDesc(sizeof(uint32_t) * kInstanceNum_),
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

	// -------------------------------------------------
	// ↓ Particleの初期化をGPUで行う
	// -------------------------------------------------

	Engine::SetCsPipeline(CsPipelineType::GpuParticleInit);
	InitBindCmdList(commandList, 0);
	commandList->Dispatch((UINT)kInstanceNum_ / 1024, 1, 1);

}

void GpuParticles::Update() {
}

void GpuParticles::InitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, particleResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 1, freeListIndexResource_->GetUAV().handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 2, freeListResource_->GetUAV().handleGPU);
}
