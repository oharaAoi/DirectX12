#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Math/MyMath.h"

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

	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

private:

	ComPtr<ID3D12Resource> materialBuffer_;
	std::map<std::string, PBRMaterialData*> materialMap_;
	PBRMaterialData* pbrMaterial_;

	Vector3 uvTranslation_;
	Vector3 uvScale_;
	Vector3 uvRotation_;
};

