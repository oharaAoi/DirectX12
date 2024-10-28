#include "Sphere.h"

Sphere::Sphere() {
}

Sphere::~Sphere() {
}

void Sphere::Init(ID3D12Device* device, const uint32_t& division) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();

	vertexCount_ = division * division * 6;
	std::vector<Mesh::VertexData> vertices;
	std::vector<uint32_t> indices;
	vertices.resize(vertexCount_);
	indices.resize(vertexCount_);

	mesh_->Init(device, vertices, indices);
	material_->Init(device);

	const float kLonEvery = float(M_PI) * 2.0f / float(division);// fai
	const float kLatEvery = float(M_PI) / float(division); // theta
	// vertexの設定
	Mesh::VertexData* vertexData = mesh_->GetVertexData();
	uint32_t start = 0;
	// indexの設定
	uint32_t* indexData = mesh_->GetIndexData();
	uint32_t startIndex = 0;

	// 緯度方向に分割
	for (uint32_t latIndex = 0; latIndex < division; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex; // theta
		// 経度方向に分割
		for (uint32_t lonIndex = 0; lonIndex < division; ++lonIndex) {
			start = (latIndex * division + lonIndex) * 4;
			float lon = lonIndex * kLonEvery; // fai
			// 頂点データを入力
			// a 左下
			vertexData[start].pos.x = std::cos(lat) * std::cos(lon);
			vertexData[start].pos.y = std::sin(lat);
			vertexData[start].pos.z = std::cos(lat) * std::sin(lon);
			vertexData[start].pos.w = 1.0f;
			vertexData[start].texcoord.x = float(lonIndex) / float(division);
			vertexData[start].texcoord.y = 1.0f - float(latIndex) / float(division);
			vertexData[start].normal = { vertexData[start].pos.x, vertexData[start].pos.y, vertexData[start].pos.z };

			// b 左上
			vertexData[start + 1].pos.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start + 1].pos.y = std::sin(lat + kLatEvery);
			vertexData[start + 1].pos.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start + 1].pos.w = 1.0f;
			vertexData[start + 1].texcoord.x = float(lonIndex) / float(division);
			vertexData[start + 1].texcoord.y = 1.0f - float(latIndex + 1) / float(division);
			vertexData[start + 1].normal = { vertexData[start + 1].pos.x, vertexData[start + 1].pos.y, vertexData[start + 1].pos.z };

			// c 右下
			vertexData[start + 2].pos.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start + 2].pos.y = std::sin(lat);
			vertexData[start + 2].pos.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start + 2].pos.w = 1.0f;
			vertexData[start + 2].texcoord.x = float(lonIndex + 1) / float(division);
			vertexData[start + 2].texcoord.y = 1.0f - float(latIndex) / float(division);
			vertexData[start + 2].normal = { vertexData[start + 2].pos.x, vertexData[start + 2].pos.y, vertexData[start + 2].pos.z };

			// d 右上
			vertexData[start + 3].pos.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[start + 3].pos.y = std::sin(lat + kLatEvery);
			vertexData[start + 3].pos.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[start + 3].pos.w = 1.0f;
			vertexData[start + 3].texcoord.x = float(lonIndex + 1) / float(division);
			vertexData[start + 3].texcoord.y = 1.0f - float(latIndex + 1) / float(division);
			vertexData[start + 3].normal = { vertexData[start + 3].pos.x, vertexData[start + 3].pos.y, vertexData[start + 3].pos.z };

			//
			startIndex = (latIndex * division + lonIndex) * 6;

			indexData[startIndex] = start;
			indexData[startIndex + 1] = start + 1;
			indexData[startIndex + 2] = start + 2;

			indexData[startIndex + 3] = start + 1;
			indexData[startIndex + 4] = start + 3;
			indexData[startIndex + 5] = start + 2;
		}

		// materialの設定
		/*Material::MaterialData* materialData = material_->GetBaseMaterial();*/
		//materialData->enableLighting = true;
	}
}

void Sphere::Update() {

}

void Sphere::Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform* worldTransform, const ViewProjection* viewProjection) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);

	worldTransform->Draw(commandList);
	viewProjection->Draw(commandList);

	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, "Resources/monsterBall.png", 3);
	commandList->DrawIndexedInstanced(vertexCount_, 1, 0, 0, 0);
}

#ifdef _DEBUG
void Sphere::ImGuiDraw(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {
		material_->ImGuiDraw();
		ImGui::TreePop();
	}
}
#endif