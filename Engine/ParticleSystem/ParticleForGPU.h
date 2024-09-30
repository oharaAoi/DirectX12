#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/Math/MyMath.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class ParticleForGPU {
public:

	struct ParticleForGPUData{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 worldInverseTranspose;
		Vector4 color;
	};

public:

	ParticleForGPU();
	~ParticleForGPU();

	void Finalize();
	void Init(ID3D12Device* device, const uint32_t& instanceSize);
	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const Vector4& color, const uint32_t& index);
	void StackCommand(ID3D12GraphicsCommandList* commandList);

	void CreateSrv(ID3D12Device* device, DescriptorHeap* dxHeap, const uint32_t& instanceNum);

public:

	ParticleForGPUData* GetParticleData() { return particleData_; }

private:

	uint32_t instanceSize_;

	ComPtr<ID3D12Resource> cBuffer_;
	ParticleForGPUData* particleData_;

	DescriptorHeap::DescriptorHandles instancingSrvHandle_;
};

