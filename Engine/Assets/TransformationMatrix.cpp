#include "TransformationMatrix.h"

TransformationMatrix::TransformationMatrix() {
}

TransformationMatrix::~TransformationMatrix() {
	Finalize();
}

void TransformationMatrix::Finalize() {
	cBuffer_.Reset();
}

void TransformationMatrix::Init(ID3D12Device* device, const uint32_t& instanceSize) {
	instanceSize_ = instanceSize;
	cBuffer_ = CreateBufferResource(device, sizeof(TransformationData) * instanceSize);
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

void TransformationMatrix::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

