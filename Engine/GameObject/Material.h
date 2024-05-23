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
	};

public:

	Material();
	~Material();

	void Init(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

private:

	Comptr<ID3D12Resource> materialBuffer_;
	ResMaterial* materialData_;

};