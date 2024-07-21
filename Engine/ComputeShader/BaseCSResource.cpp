#include "BaseCSResource.h"

void BaseCSResource::Finalize() {
	uavBuffers_.clear();
	cBuffer_.Reset();
}

void BaseCSResource::Init(ID3D12Device* device, DescriptorHeap* dxHeap) {
	device_ = device;
	dxHeap_ = dxHeap;

	writeResourceHandles_ = uavBuffers_[0].uavAddress;
}

void BaseCSResource::SetResource(ID3D12GraphicsCommandList* commandList) {
	commandList->SetComputeRootDescriptorTable(0, referenceResourceHandles_.handleGPU);
	commandList->SetComputeRootDescriptorTable(1, writeResourceHandles_.handleGPU);
	commandList->SetComputeRootConstantBufferView(2, cBuffer_->GetGPUVirtualAddress());
}

void BaseCSResource::TransitionUAVResource(ID3D12GraphicsCommandList* commandList, 
										   const D3D12_RESOURCE_STATES& beforState,
										   const D3D12_RESOURCE_STATES& afterState,
										   const uint32_t& index) {

	TransitionResourceState(commandList, uavBuffers_[index].uavBuffer.Get(), beforState, afterState);
}

void BaseCSResource::CreateResourceBuffer(const uint32_t& createNum) {
	for (uint32_t oi = 0; oi < createNum; oi++) {
		UavBufferData bufferData;

		D3D12_RESOURCE_DESC desc{};
		desc.Width = kWindowWidth_;			// 画面の横幅
		desc.Height = kWindowHeight_;			// 画面の縦幅
		desc.MipLevels = 1;			// 
		desc.DepthOrArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		// HEAPの設定
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		// Resourceの作成
		bufferData.uavBuffer = CerateShaderResource(device_, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		// ------------------------------------------------------------
		// UAVの設定
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Format = bufferData.uavBuffer->GetDesc().Format;
		// SRVを作成するDescriptorHeapの場所を求める
		bufferData.uavAddress = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
		// 生成
		device_->CreateUnorderedAccessView(bufferData.uavBuffer.Get(), nullptr, &uavDesc, bufferData.uavAddress.handleCPU);

		//// ------------------------------------------------------------
		//// SRVの設定
		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1;
		//// SRVを作成するDescriptorHeapの場所を求める
		//bufferData.srvAddress = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
		//// 生成
		//device_->CreateShaderResourceView(bufferData.uavBuffer.Get(), &srvDesc, bufferData.srvAddress.handleCPU);

		uavBuffers_.push_back(std::move(bufferData));
	}
}
