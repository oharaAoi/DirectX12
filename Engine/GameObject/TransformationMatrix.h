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

	void Init(ID3D12Device* device);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	void Draw(ID3D12GraphicsCommandList* commandList);

public:

	ResTransformationMatrix* GetTransformationData() { return TransformationData_; }

private:

	Comptr<ID3D12Resource> cBuffer_;
	ResTransformationMatrix* TransformationData_;
};