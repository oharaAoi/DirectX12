#include "GpuParticle.h"

GpuParticle::GpuParticle() {
}

GpuParticle::~GpuParticle() {
}

void GpuParticle::Init(const std::string& modelName, uint32_t instanceNum) {
	// ポインタの取得
	DescriptorHeap* dxHeap = Engine::GetDxHeap();
	ID3D12Device* dxDevice = Engine::GetDevice();
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();

	kInstanceNum_ = instanceNum;

	// materilとmeshの設定
	materialArray_ = LoadMaterialData(ModelManager::GetModelPath(modelName), modelName, dxDevice);
	meshArray_ = LoadVertexData(ModelManager::GetModelPath(modelName), modelName, dxDevice);
	// gpuに送るResourceの作成

	perViewBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerView));
	perViewBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perView_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	particleBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(Particle) * kInstanceNum_);
	// UAVとSRVの作成
	// ------------------------------------------------------------
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = kInstanceNum_;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(Particle);
	uav_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateUnorderedAccessView(particleBuffer_.Get(), nullptr, &uavDesc, uav_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // 頂点データなのでフォーマットはUNKNOWN
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(kInstanceNum_);  // 頂点の数
	srvDesc.Buffer.StructureByteStride = sizeof(Particle);  // 頂点1つあたりのサイズ
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srv_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateShaderResourceView(particleBuffer_.Get(), &srvDesc, srv_.handleCPU);

	// -------------------------------------------------
	// ↓ Particleの初期化をGPUで行う
	// -------------------------------------------------
	particleCounter_ = CreateUAVResource(Engine::GetDevice(), sizeof(int) * kInstanceNum_);

	// UAVとSRVの作成
	// ------------------------------------------------------------
	// UAVの設定
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = kInstanceNum_;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(int);
	counterUav_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateUnorderedAccessView(particleCounter_.Get(), nullptr, &uavDesc, counterUav_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // 頂点データなのでフォーマットはUNKNOWN
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(kInstanceNum_);  // 頂点の数
	srvDesc.Buffer.StructureByteStride = sizeof(int);  // 頂点1つあたりのサイズ
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	counterSrv_ = dxHeap->AllocateSRV();
	// 生成
	dxDevice->CreateShaderResourceView(particleCounter_.Get(), &srvDesc, counterSrv_.handleCPU);

	// -------------------------------------------------
	// ↓ Particleの初期化をGPUで行う
	// -------------------------------------------------
	
	Engine::SetCsPipeline(CsPipelineType::GpuParticleInit);
	BindCmdList(commandList, 0);
	commandList->Dispatch((UINT)kInstanceNum_ / 1024, 1, 1);
}

void GpuParticle::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	perView_->viewProjection = Render::GetViewport3D() * Render::GetProjection3D();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	Engine::SetCsPipeline(CsPipelineType::GpuParticleUpdate);
	commandList->SetComputeRootDescriptorTable(0, uav_.handleGPU);
	commandList->SetComputeRootConstantBufferView(1, perFrameBuffer_->GetGPUVirtualAddress());
	commandList->Dispatch((UINT)kInstanceNum_ / 1024, 1, 1);

	// UAVの変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = particleBuffer_.Get();
	commandList->ResourceBarrier(1, &barrier);

}

void GpuParticle::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materialArray_[meshArray_[oi]->GetUseMaterial()]->Draw(commandList);
		commandList->SetGraphicsRootDescriptorTable(1, srv_.handleGPU);
		commandList->SetGraphicsRootConstantBufferView(4, perViewBuffer_->GetGPUVirtualAddress());

		std::string textureName = materialArray_[meshArray_[oi]->GetUseMaterial()]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 2);

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), kInstanceNum_, 0, 0, 0);
	}
}

void GpuParticle::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, uav_.handleGPU);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex + 1, counterUav_.handleGPU);
}
