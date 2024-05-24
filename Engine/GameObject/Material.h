#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include "DirectXUtils.h"
// math
#include "MyMath.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class Material {
public:

	struct ResMaterial {
		Vector4 color;
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
	};

public:

	Material();
	~Material();

	void Init(ID3D12Device* device);

	void Update(const Matrix4x4& uvTransform);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

public:

	ResMaterial* GetMaterialData() { return materialData_; }

private:

	Comptr<ID3D12Resource> materialBuffer_;
	ResMaterial* materialData_;

};