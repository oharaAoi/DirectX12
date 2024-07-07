#include "ParticleForGPU.h"

ParticleForGPU::ParticleForGPU() {
}

ParticleForGPU::~ParticleForGPU() {
	Finalize();
}

void ParticleForGPU::Finalize() {
}

void ParticleForGPU::Init(ID3D12Device* device, const uint32_t& instanceSize) {
	instanceSize_ = instanceSize;
	cBuffer_ = CreateBufferResource(device, sizeof(ParticleForGPUData) * instanceSize);
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&particleData_));
}

void ParticleForGPU::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const Vector4& color) {
	for (uint32_t oi = 0; oi < instanceSize_; oi++) {
		particleData_[oi].world = world;
		particleData_[oi].view = view;
		particleData_[oi].projection = projection;
		particleData_[oi].worldInverseTranspose = Transpose(Inverse(world));
		particleData_[oi].color = color;
	}
}

void ParticleForGPU::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const Vector4& color, const uint32_t& index) {
	particleData_[index].world = world;
	particleData_[index].view = view;
	particleData_[index].projection = projection;
	particleData_[index].worldInverseTranspose = Transpose(Inverse(world));
	particleData_[index].color = color;
}

void ParticleForGPU::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

void ParticleForGPU::DrawSRV(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);
}

void ParticleForGPU::CreateSrv(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize, const uint32_t& index, const uint32_t& instanceNum) {
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = instanceNum;
	desc.Buffer.StructureByteStride = sizeof(ParticleForGPUData);

	instancingSrvHandleCPU_ = GetCPUDescriptorHandle(srvHeap, srvDescriptorSize, index + 1);
	instancingSrvHandleGPU_ = GetGPUDescriptorHandle(srvHeap, srvDescriptorSize, index + 1);

	device->CreateShaderResourceView(cBuffer_.Get(), &desc, instancingSrvHandleCPU_);
}
