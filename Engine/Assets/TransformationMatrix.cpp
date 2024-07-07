#include "TransformationMatrix.h"

TransformationMatrix::TransformationMatrix() {
}

TransformationMatrix::~TransformationMatrix() {
	Finalize();
}

void TransformationMatrix::Finalize() {
	
}

void TransformationMatrix::Init(ID3D12Device* device, const uint32_t& instanceSize) {
	instanceSize_ = instanceSize;
	cBuffer_ = CreateBufferResource(device, sizeof(ResTransformationMatrix) * instanceSize);
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&TransformationData_));

	//*TransformationData_ = MakeIdentity4x4();
}

void TransformationMatrix::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection) {
	for (uint32_t oi = 0; oi < instanceSize_; oi++) {
		TransformationData_[oi].world = world;
		TransformationData_[oi].view = view;
		TransformationData_[oi].projection = projection;
		TransformationData_[oi].worldInverseTranspose = Transpose(Inverse(world));
	}
}

void TransformationMatrix::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const uint32_t& index) {
	TransformationData_[index].world = world;
	TransformationData_[index].view = view;
	TransformationData_[index].projection = projection;
	TransformationData_[index].worldInverseTranspose = Transpose(Inverse(world));
}

void TransformationMatrix::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

void TransformationMatrix::DrawSRV(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);
}

void TransformationMatrix::CreateSrv(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize, const uint32_t& idnex) {
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = 10;
	desc.Buffer.StructureByteStride = sizeof(ResTransformationMatrix);

	instancingSrvHandleCPU_ = GetCPUDescriptorHandle(srvHeap, srvDescriptorSize, idnex + 1);
	instancingSrvHandleGPU_ = GetGPUDescriptorHandle(srvHeap, srvDescriptorSize, idnex + 1);

	device->CreateShaderResourceView(cBuffer_.Get(), &desc, instancingSrvHandleCPU_);
}
