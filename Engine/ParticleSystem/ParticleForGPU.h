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

class ParticleForGPU {
public:

	struct ParticleForGPUData{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Vector4 color;
	};

public:

	ParticleForGPU();
	~ParticleForGPU();

	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="instanceSize"></param>
	void Init(ID3D12Device* device, const uint32_t& instanceSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="world"></param>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	/// <param name="color"></param>
	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const Vector4& color);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection, const Vector4& color, const uint32_t& index);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

	void DrawSRV(ID3D12GraphicsCommandList* commandList);

	void CreateSrv(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize, const uint32_t& idnex, const uint32_t& instanceNum);

public:

	ParticleForGPUData* GetParticleData() { return particleData_; }

private:

	uint32_t instanceSize_;

	Comptr<ID3D12Resource> cBuffer_;
	ParticleForGPUData* particleData_;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;
};

