#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include "DirectXUtils.h"
// math
#include "MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class Material {
public:

	struct BaseMaterial {
		Vector4 color;
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
		// 拡散反射率
		float aldedo;
		// 屈折率
		float refractiveIndex;
		float padding[2];
	};

	struct MaterialData {
		float specularExponent; // ハイライトのシャープさ
		Vector3 diffuse;		// 拡散光の色
		Vector3 specularColor;	// 鏡面反射の色
		Vector3 emissiveColor;	// 自己発光の色
		float refraction;		// 屈折率
		float opacity;			// 不透明度
		std::string textureFilePath; // 使用するtextureのパス
	};

public:

	Material();
	~Material();

	void Init(ID3D12Device* device);

	void Update(const Matrix4x4& uvTransform);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

	void ImGuiDraw();

public:

	BaseMaterial* GetBaseMaterial() { return baseMaterial_; }

	MaterialData GetMateriaData() { return materialData_; }

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