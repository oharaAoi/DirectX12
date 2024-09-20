#include "PBRMaterial.h"

PBRMaterial::PBRMaterial() {}
PBRMaterial::~PBRMaterial() { Finalize(); }

void PBRMaterial::Finalize() {
	materialBuffer_.Reset();
	for (auto& entry : materialMap_) {
		delete entry.second; // PBRMaterialData* を解放
	}
	materialMap_.clear();
}

void PBRMaterial::Init(ID3D12Device* device) {
	materialBuffer_ = CreateBufferResource(device, sizeof(PBRMaterialData) * materialMap_.size());
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialMap_));

	for (auto it = materialMap_.begin(); it != materialMap_.end(); ++it) {
		PBRMaterialData* material = it->second;

		material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		material->enableLighting = false;
		material->uvTransform = MakeIdentity4x4();
		
		material->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		material->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		
		material->roughness = 0.5f;
		material->metallic = 0.5f;
		material->shininess = 50;
	}
}

void PBRMaterial::Update() {
	
}

void PBRMaterial::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

void PBRMaterial::LoadMaterial(const std::string& directoryPath, const std::string& fileName) {
	
	// objファイルからmtlファイルを探す
	std::ifstream file(directoryPath + "/" + fileName);
	assert(file.is_open());

	std::string line;// ファイルから読み込んだ1行を格納する
	std::string mtlFile;
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		if (identifier == "mtllib") {
			std::string mtlFileName;
			s >> mtlFileName;

			mtlFile = mtlFileName;
		}
	}

	// mtlファイルを開く
	std::ifstream opneMtlFile(directoryPath + "/" + mtlFile);
	assert(opneMtlFile.is_open());

	while (std::getline(opneMtlFile, line)) {
		std::string materialIdentifier;
		std::istringstream s(line);
		s >> materialIdentifier; // 先頭の識別子を読む

		if (materialIdentifier == "newmtl") {
			std::string materialName;
			s >> materialName;
			materialMap_[materialName] = new PBRMaterialData();

		} else if (materialIdentifier == "map_Kd") {
			// テクスチャマップを読み取る
			std::string textureFilename;
			s >> textureFilename;
		

		} else if (materialIdentifier == "Ka") {
			// アルベド色を読み取る(環境反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			
		} else if (materialIdentifier == "Kd") {
			// ディフューズ色を読み取る(拡散反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			
		} else if (materialIdentifier == "Ks") {
			// スペキュラ色(鏡面反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			
		} else if (materialIdentifier == "Ke") {
			// 自己発光
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			
		} else if (materialIdentifier == "Ni") {
			// 屈折率
			float refraction;
			s >> refraction;
			
		} else if (materialIdentifier == "Ns") {
			// shininess(鏡面反射の鋭さ)
		}
	}
}
