#include "GaussianBlur.h"

GaussianBlur::GaussianBlur(const UINT& gpuGroupCountX, const UINT& gpuGroupCountY,
						   ComputeShaderPipeline* horizontal, ComputeShaderPipeline* vertical) {
	groupCountX_ = gpuGroupCountX;
	groupCountY_ = gpuGroupCountY;

	verticalPipeline_ = vertical;
	horizontalPipeline_ = horizontal;
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

	// 1こ目を水平方向のresourceにする。2こ目を垂直方向に
	BaseCSResource::CreateResourceBuffer(2);
	BaseCSResource::Init(device, dxHeap);

	// cBufferの作成
	cBuffer_ = CreateBufferResource(device_, sizeof(GaussianBlurData));

	data_ = nullptr;
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->blurStrength = 1.0f;
	data_->kernelSize = 5;
	data_->direction = {1.0f, 1.0f};

	usePipelineType_ = CsPipelineType::HorizontalBlur_Pipeline;
}

void GaussianBlur::ConfigureResource(ID3D12GraphicsCommandList* commandList) {
#ifdef _DEBUG
	ImGui::Begin("PostEffect");
	ImGui::DragFloat("blurStrength", &data_->blurStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("kernelSize", &data_->kernelSize, 0.01f, 1.0f, 10.0f);
	ImGui::DragFloat2("direction", &data_->direction.x, 0.01f, 0.0f, 10.0f);
	ImGui::End();
#endif

	// -------------------------------------------------
	// ↓ 水平方向のブラーを掛ける
	// -------------------------------------------------
	horizontalPipeline_->SetPipelineState(commandList);
	BaseCSResource::ConfigureResource(commandList);
	commandList->Dispatch(groupCountX_, groupCountY_, 1);

	// -------------------------------------------------
	// ↓ 水平方向に掛けたブラーのリソースの状態を変更する
	// -------------------------------------------------
	TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, 0);
	referenceResourceHandles_ = bufferHandles_[0].srvAddress;
	writeResourceHandles_ = bufferHandles_[1].uavAddress;

	// -------------------------------------------------
	// ↓ 垂直方向のブラーを掛ける
	// -------------------------------------------------
	verticalPipeline_->SetPipelineState(commandList);
	BaseCSResource::ConfigureResource(commandList);
	commandList->Dispatch(groupCountX_, groupCountY_, 1);

	writeResourceHandles_ = bufferHandles_[0].uavAddress;
	TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, 1);
}

