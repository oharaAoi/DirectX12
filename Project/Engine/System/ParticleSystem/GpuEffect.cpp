#include "GpuEffect.h"
#include "Engine/Engine.h"
#include "Engine/System/ParticleSystem/Emitter/SphereEmitter.h"
#include "Engine/System/ParticleSystem/Emitter/ConeEmitter.h"
#include "Engine/System/ParticleSystem/Emitter/BoxEmitter.h"

GpuEffect::GpuEffect() {}
GpuEffect::~GpuEffect() {
	allDeathBuffer_.Reset();
	outputBuffer_.Reset();
	DescriptorHeap::AddFreeSrvList(allDeathSrv_.assignIndex_);
	DescriptorHeap::AddFreeSrvList(allDeathUav_.assignIndex_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Init(const EmitterShape& shape) {
	gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024);

	switch (shape) {
	case EmitterShape::Sphere:
		gpuEmitter_ = std::make_unique<SphereEmitter>();
		break;
	case EmitterShape::Cone:
		gpuEmitter_ = std::make_unique<ConeEmitter>();
		break;
	case EmitterShape::Box:
		gpuEmitter_ = std::make_unique<BoxEmitter>();
		break;
	default:
		break;
	}
	
	gpuEmitter_->Init();

	// ポインタの取得
	DescriptorHeap* dxHeap = Engine::GetDxHeap();
	ID3D12Device* dxDevice = Engine::GetDevice();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	allDeathBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(uint32_t));
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	allDeathUav_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateUnorderedAccessView(allDeathBuffer_.Get(), nullptr, &uavDesc, allDeathUav_.handleCPU);

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // 頂点データなのでフォーマットはUNKNOWN
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;  // 頂点の数
	srvDesc.Buffer.StructureByteStride = sizeof(uint32_t);  // 頂点1つあたりのサイズ
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	allDeathSrv_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateShaderResourceView(allDeathBuffer_.Get(), &srvDesc, allDeathSrv_.handleCPU);

	// 読み取りよう
	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_READBACK);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(uint32_t));

	dxDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&outputBuffer_)
	);

	isAlive_ = true;
	allAlphaZero_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Update() {
	/*if (gpuEmitter_->GetIsDead()) {
		isAlive_ = false;
		return;
	}*/
	gpuParticle_->SetPerViewProjectionMat(viewProjectionMat_);

	gpuEmitter_->Update();
	gpuParticle_->Update();

	if (!gpuEmitter_->GetIsDead()) {
		ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
		Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
		gpuParticle_->EmitBindCmdList(commandList, 0);
		gpuEmitter_->BindCmdList(commandList, 2);
		Dispatch(commandList, Vector3(1, 1, 1));
	}

	// particleが消えた方の確認
	if (gpuEmitter_->GetIsDead()) {
		CheckParticlesAlpha();
	}

	if (allAlphaZero_) {
		isAlive_ = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Draw() const {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	gpuParticle_->Draw(Engine::GetCommandList());

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	if (!gpuEmitter_->GetIsDead()) {
		gpuEmitter_->DrawShape(viewProjectionMat_);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CSの実行命令
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group) {
	commandList->Dispatch((UINT)group.x, (UINT)group.y, (UINT)group.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Particleがすべてきえているか確認
//////////////////////////////////////////////////////////////////////////////////////////////////

void GpuEffect::CheckParticlesAlpha() {
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	// dispatch
	Engine::SetCsPipeline(CsPipelineType::GpuParticleEnd);
	gpuParticle_->EndBindCmdList(commandList, 0);
	commandList->SetComputeRootDescriptorTable(1, allDeathUav_.handleGPU);
	commandList->Dispatch(1,1,1);

	// resourceを読みとる
	TransitionResourceState(commandList, allDeathBuffer_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList->CopyResource(outputBuffer_.Get(), allDeathBuffer_.Get());
	TransitionResourceState(commandList, allDeathBuffer_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	uint32_t* mappedData = nullptr;
	outputBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));
	
	allAlphaZero_ = static_cast<uint32_t>(mappedData[0]);

	outputBuffer_->Unmap(0, nullptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GpuEffect::Debug_Gui() {
	gpuEmitter_->Debug_Gui();
	gpuEmitter_->SelectEmitModel();
	if (ImGui::Button("Reset")) {
		gpuParticle_->ResetModel(gpuEmitter_->GetParticleModel());
	}
}
#endif