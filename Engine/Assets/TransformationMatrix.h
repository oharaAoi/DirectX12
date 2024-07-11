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

	struct TransformationData {
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 worldInverseTranspose;
	};

public:

	TransformationMatrix();
	~TransformationMatrix();

	void Finalize();

	void Init(ID3D12Device* device, const uint32_t& instanceSize);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	void Draw(ID3D12GraphicsCommandList* commandList);

public:

	TransformationData* GetTransformationData() { return TransformationData_; }

private:

	uint32_t instanceSize_;

	Comptr<ID3D12Resource> cBuffer_;
	TransformationData* TransformationData_;
};