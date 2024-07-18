#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() {
}

GaussianBlur::~GaussianBlur() {
}

void GaussianBlur::Finalize() {
	BaseCSResource::Finalize();
}

void GaussianBlur::Init(ID3D12Device* device, DescriptorHeap* dxHeap) {
	assert(device);
	assert(dxHeap);

	dxHeap_ = dxHeap;
	device_ = device;

	BaseCSResource::Init(device, dxHeap);
	// 1こ目を水平方向のresourceにする。2こ目を垂直方向に
	BaseCSResource::CreateUAVBuffer(2);

	// cBufferの作成
	cBuffer_ = CreateBufferResource(device_, sizeof(GaussianBlurData));

	data_ = nullptr;
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->blurStrength = 1.0f;
	data_->kernelSize = 5;
	data_->direction = {1.0f, 1.0f};
}

void GaussianBlur::HorizontalSetResource(ID3D12GraphicsCommandList* commandList) {

	ImGui::Begin("PostEffect");
	ImGui::DragFloat("blurStrength", &data_->blurStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("kernelSize", &data_->kernelSize, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat2("direction", &data_->direction.x, 0.01f, 0.0f, 10.0f);
	ImGui::End();

	BaseCSResource::SetResource(commandList, 0, 1);
	commandList->SetComputeRootConstantBufferView(2, cBuffer_->GetGPUVirtualAddress());
}

void GaussianBlur::VerticalSetResource(ID3D12GraphicsCommandList* commandList) {
	BaseCSResource::SetResource(commandList, 0, 0);
	BaseCSResource::SetResource(commandList, 1, 1);
	commandList->SetComputeRootConstantBufferView(2, cBuffer_->GetGPUVirtualAddress());
}

void GaussianBlur::SetResultResource(ID3D12GraphicsCommandList* commandList) {
	BaseCSResource::SetResource(commandList, 1, 0);
}
