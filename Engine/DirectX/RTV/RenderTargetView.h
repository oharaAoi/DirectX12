#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class RenderTargetView {
public:

	RenderTargetView();
	~RenderTargetView();

private:

	// descriptorHeap
	//Comptr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;

	// rtv
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
};