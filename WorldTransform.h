#pragma once
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

struct WorldTransformData {
	Matrix4x4 matWorld;
	Matrix4x4 worldInverseTranspose;
};

class WorldTransform {
public:

	Vector3 scale_;
	Vector3 rotation_;
	Vector3 translation_;

public:

	WorldTransform();
	~WorldTransform();

	void Init(ID3D12Device* device);

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList) const;

	void Finalize();

private:

	Comptr<ID3D12Resource> cBuffer_;
	WorldTransformData* data_;
};

