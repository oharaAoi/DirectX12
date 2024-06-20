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
		Vector4 color;				// albedo
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;

		Vector4 diffuseColor;		// 拡散反射率
		Vector4 specularColor;		// 鏡面反射の色
		float roughness;			// 粗さ
		float metallic;				// 金属度
		float shininess;			// 鋭さ
	};

	struct MaterialData {
		float specularExponent; // ハイライトのシャープさ
		Vector4 albedo;			// 環境光の色
		Vector4 diffuse;		// 拡散光の色
		Vector4 specular;	// 鏡面反射の色
		Vector4 emissive;	// 自己発光の色
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
	void SetMaterialData(MaterialData materialData);

	void SetMaterialParameter(const float& roughness, const float& metallic);

private:

	Comptr<ID3D12Resource> materialBuffer_;
	BaseMaterial* baseMaterial_;

	// materialData
	MaterialData materialData_;
};