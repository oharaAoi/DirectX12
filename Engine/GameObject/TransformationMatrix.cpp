#include "TransformationMatrix.h"

TransformationMatrix::TransformationMatrix() {
}

TransformationMatrix::~TransformationMatrix() {
	Finalize();
}

void TransformationMatrix::Finalize() {
	cBuffer_.Reset();
}

void TransformationMatrix::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(ResTransformationMatrix));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&TransformationData_));
}

void TransformationMatrix::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection) {
	TransformationData_->world = world;
	TransformationData_->view = view;
	TransformationData_->projection = projection;
}

void TransformationMatrix::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}
