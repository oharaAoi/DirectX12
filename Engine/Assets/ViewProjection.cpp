#include "ViewProjection.h"

ViewProjection::ViewProjection() {
}

ViewProjection::~ViewProjection() {
}

void ViewProjection::Finalize() {
	cBuffer_.Reset();
}

void ViewProjection::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(ViewProjectionData));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
}

void ViewProjection::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(2, cBuffer_->GetGPUVirtualAddress());
}