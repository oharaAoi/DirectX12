#include "PBRMaterial.h"

PBRMaterial::PBRMaterial() {}
PBRMaterial::~PBRMaterial() { Finalize(); }

void PBRMaterial::Finalize() {
	materialBuffer_.Reset();
}

void PBRMaterial::Init(ID3D12Device* device) {
	materialBuffer_ = CreateBufferResource(device, sizeof(PBRMaterialData));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&pbrMaterial_));

	pbrMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->enableLighting = false;
	pbrMaterial_->uvTransform = MakeIdentity4x4();

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;
	pbrMaterial_->shininess = 10;

	uvTranslation_ = {0.0f, 0.0f, 0.0f};
	uvScale_ = { 0.2f, 0.2f, 1.0f };;
	uvRotation_ = { 0.0f, 0.0f, 0.0f };
}

void PBRMaterial::Update() {

}

void PBRMaterial::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void PBRMaterial::Debug_Gui() {
	if (ImGui::TreeNode("uvTramsform")) {
		if (ImGui::TreeNode("scale")) {
			ImGui::DragFloat4("uvScale", &uvScale_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("rotation")) {
			ImGui::DragFloat4("uvRotation", &uvRotation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("translation")) {
			ImGui::DragFloat4("uvTranslation", &uvTranslation_.x, 0.01f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::DragFloat("roughness", &pbrMaterial_->roughness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("metallic", &pbrMaterial_->metallic, 0.01f, 0.0f, 1.0f);
	//ImGui::DragFloat("shininess", &pbrMaterial_->shininess, 0.1f);

	pbrMaterial_->uvTransform = MakeAffineMatrix(uvScale_, uvRotation_, uvTranslation_);
}
#endif

void PBRMaterial::SetMaterialData(Model::ModelMaterialData materialData) {
	materials_ = materialData;

	pbrMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->enableLighting = true;
	pbrMaterial_->uvTransform = MakeIdentity4x4();

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 1.0f;
	pbrMaterial_->metallic = 1.0f;
	pbrMaterial_->shininess = 50;
}


//void PBRMaterial::LoadMaterial(const std::string& directoryPath, const std::string& fileName) {
//	
//	// objファイルからmtlファイルを探す
//	std::ifstream file(directoryPath + "/" + fileName);
//	assert(file.is_open());
//
//	std::string line;// ファイルから読み込んだ1行を格納する
//	std::string mtlFile;
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier; // 先頭の識別子を読む
//
//		if (identifier == "mtllib") {
//			std::string mtlFileName;
//			s >> mtlFileName;
//
//			mtlFile = mtlFileName;
//		}
//	}
//
//	// mtlファイルを開く
//	std::ifstream opneMtlFile(directoryPath + "/" + mtlFile);
//	assert(opneMtlFile.is_open());
//
//	while (std::getline(opneMtlFile, line)) {
//		std::string materialIdentifier;
//		std::istringstream s(line);
//		s >> materialIdentifier; // 先頭の識別子を読む
//
//		if (materialIdentifier == "newmtl") {
//			std::string materialName;
//			s >> materialName;
//			materialMap_[materialName] = new PBRMaterialData();
//
//		} else if (materialIdentifier == "map_Kd") {
//			// テクスチャマップを読み取る
//			std::string textureFilename;
//			s >> textureFilename;
//		
//
//		} else if (materialIdentifier == "Ka") {
//			// アルベド色を読み取る(環境反射率)
//			Vector4 color;
//			s >> color.x >> color.y >> color.z;
//			
//		} else if (materialIdentifier == "Kd") {
//			// ディフューズ色を読み取る(拡散反射率)
//			Vector4 color;
//			s >> color.x >> color.y >> color.z;
//			
//		} else if (materialIdentifier == "Ks") {
//			// スペキュラ色(鏡面反射率)
//			Vector4 color;
//			s >> color.x >> color.y >> color.z;
//			
//		} else if (materialIdentifier == "Ke") {
//			// 自己発光
//			Vector4 color;
//			s >> color.x >> color.y >> color.z;
//			
//		} else if (materialIdentifier == "Ni") {
//			// 屈折率
//			float refraction;
//			s >> refraction;
//			
//		} else if (materialIdentifier == "Ns") {
//			// shininess(鏡面反射の鋭さ)
//		}
//	}
//}