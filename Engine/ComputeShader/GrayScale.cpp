#include "GrayScale.h"

GrayScale::GrayScale() {}
GrayScale::~GrayScale() {}

void GrayScale::Finalize() {
	BaseCSResource::Finalize();
}

void GrayScale::Init(ID3D12Device* device, DescriptorHeap* dxHeap) {
	assert(device);
	assert(dxHeap);

	dxHeap_ = dxHeap;
	device_ = device;

	// UAVのResource作成とViewの作成
	BaseCSResource::Init(device, dxHeap);
	CreateUAVBuffer(1);

	// cBufferの作成
	cBuffer_ = CreateBufferResource(device_, sizeof(GrayScaleData));
	
	data_ = nullptr;
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->grayScaleAmount = 0.0f;
}

void GrayScale::Draw(ID3D12GraphicsCommandList* commandList) {
	ImGui::Begin("PostEffect");
	ImGui::DragFloat("GrayScale", &data_->grayScaleAmount, 0.01f, 0.0f, 1.0f);
	ImGui::End();

	BaseCSResource::Draw(commandList, 0);
	commandList->SetComputeRootConstantBufferView(2, cBuffer_->GetGPUVirtualAddress());
}

void GrayScale::TransitionResource(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState) {
	TransitionResourceState(commandList, uavBuffers_[0].uavBuffer.Get(), beforState, afterState);
}
