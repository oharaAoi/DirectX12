#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Math/MyMath.h"
#include "Engine/GameObject/Model.h"

class PBRMaterial {
public:

	struct PBRMaterialData {
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

public:

	PBRMaterial();
	~PBRMaterial();

	void Finalize();
	void Init(ID3D12Device* device);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

	const std::string GetUseTexture() const { return materials_.textureFilePath; }

	void SetColor(const Vector4& color) { pbrMaterial_->color = color; }

	void SetMaterialData(Model::ModelMaterialData materialData);

	void SetUvScale(const Vector3& scale) { uvScale_ = scale; };
	void SetUvTransition(const Vector3& transition) { uvTranslation_ = transition; };

private:

	ComPtr<ID3D12Resource> materialBuffer_;
	PBRMaterialData* pbrMaterial_;

	Model::ModelMaterialData materials_;

	Vector3 uvTranslation_;
	Vector3 uvScale_;
	Vector3 uvRotation_;
};

