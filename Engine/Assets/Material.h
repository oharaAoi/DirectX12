#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/MyMath.h"

#include "Engine/Manager/ImGuiManager.h"


template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class Material {
public:

	struct BaseMaterial {
		Vector4 color;				// albedo
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	struct PBRMaterial {
		Vector4 color;				// albedo
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
		float shininess;			// 鋭さ
		
		Vector4 diffuseColor;		// 拡散反射率
		Vector4 specularColor;		// 鏡面反射の色
		float roughness;			// 粗さ
		float metallic;				// 金属度
	};

	struct ModelMaterialData {
		float specularExponent; // ハイライトのシャープさ
		Vector4 albedo;			// 環境光の色
		Vector4 diffuse;		// 拡散光の色
		Vector4 specular;		// 鏡面反射の色
		Vector4 emissive;		// 自己発光の色
		float refraction;		// 屈折率
		float opacity;			// 不透明度
		std::string textureFilePath; // 使用するtextureのパス
	};

	struct SpriteData {
		Matrix4x4 uvTransform;
		Vector4 color;
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

	PBRMaterial* GetBaseMaterial() { return pbrMaterial_; }

	ModelMaterialData GetMateriaData() { return materialData_; }

	/// <summary>
	/// materialDataをセットする
	/// </summary>
	/// <param name="materialData"></param>
	void SetMaterialData(ModelMaterialData materialData);

	void SetMaterialParameter(const float& roughness, const float& metallic);

private:

	ComPtr<ID3D12Resource> materialBuffer_;
	PBRMaterial* pbrMaterial_;

	// materialData
	ModelMaterialData materialData_;

	Vector3 uvTranslation_;
	Vector3 uvScale_;
	Vector3 uvRotation_;


};