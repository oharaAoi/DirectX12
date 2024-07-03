#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "DirectXUtils.h"
// math
#include "MyMath.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class TransformationMatrix {
public:

	struct ResTransformationMatrix {
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
	};

public:

	TransformationMatrix();
	~TransformationMatrix();

	void Finalize();

	void Init(ID3D12Device* device, const uint32_t& instanceSize);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const uint32_t& index);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void DrawSRV(ID3D12GraphicsCommandList* commandList);

	void CreateSrv(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize, const uint32_t& idnex);

public:

	ResTransformationMatrix* GetTransformationData() { return TransformationData_; }

private:

	uint32_t instanceSize_;

	Comptr<ID3D12Resource> cBuffer_;
	ResTransformationMatrix* TransformationData_;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;
};