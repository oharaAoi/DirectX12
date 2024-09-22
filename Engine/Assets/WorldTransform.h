#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/MyMath.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Math/Quaternion.h"


template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

struct WorldTransformData {
	Matrix4x4 matWorld;
	Matrix4x4 worldInverseTranspose;
};

class WorldTransform {
public:

	Vector3 scale_;
	Quaternion rotation_;
	Vector3 translation_;

public:

	WorldTransform();
	~WorldTransform();

	void Init(ID3D12Device* device);

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList) const;

	void ImGuiDraw();

	void Finalize();

	/// <summary>
	/// gltfファイルで読み込んだモデルのLoacalMatを適応させる
	/// </summary>
	/// <param name="mat"></param>
	void AdaptToGLTF(const Matrix4x4& mat) const;

private:

	ComPtr<ID3D12Resource> cBuffer_;
	WorldTransformData* data_;

	float test_angle_ = 0;
};

