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

	struct BaseMaterial {
		Vector4 color;
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
	};

	struct MaterialData {
		Vector3 diffuse;
		std::string textureFilePath; // 使用するtextureのパス
	};

public:

	Material();
	~Material();

	void Init(ID3D12Device* device);

	void Update(const Matrix4x4& uvTransform);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

public:

	BaseMaterial* GetMaterialData() { return baseMaterial_; }

	/// <summary>
	/// materialDataをセットする
	/// </summary>
	/// <param name="materialData"></param>
	void SetMaterialData(MaterialData materialData) { materialData_ = materialData; }

private:

	Comptr<ID3D12Resource> materialBuffer_;
	BaseMaterial* baseMaterial_;

	// materialData
	MaterialData materialData_;
};