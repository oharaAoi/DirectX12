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

	// UAVのResource作成とViewの作成(今回の場合は横ブラーをかけた後縦ブラーをかけてものがUAVに当たる)
	BaseCSResource::Init(device, dxHeap);

	// cBufferの作成
	cBuffer_ = CreateBufferResource(device_, sizeof(GaussianBlurData));

	data_ = nullptr;
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
}

void GaussianBlur::Draw(ID3D12GraphicsCommandList* commandList) {
}
