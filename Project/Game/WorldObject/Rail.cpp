#include "Rail.h"

Rail::Rail() {
}

Rail::~Rail() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Init() {
	//// MeshのVertexDataを作成する
	//Mesh::VertexData vertex;
	//// 左上の実装
	//vertex.pos = Vector4{ -1, 0.0f, 1.0f, 1.0f };
	//vertex.texcoord = { 0.0f, 0.0f };
	//vertex.normal = { 0.0f, 1.0f, 0.0f };
	//vertex.tangent = { 0.0f, 0.0f, 0.0f };
	//vertexData_.push_back(vertex);
	//// 右上
	//vertex.pos = Vector4{ 1, 0.0f, 1.0f, 1.0f };
	//vertex.texcoord = { 1.0f, 0.0f };
	//vertex.normal = { 0.0f, 1.0f, 0.0f };
	//vertex.tangent = { 0.0f, 0.0f, 0.0f };
	//vertexData_.push_back(vertex);
	//// 左下
	//vertex.pos = Vector4{ -1, 0.0f, -1.0f, 1.0f };
	//vertex.texcoord = { 0.0f, 1.0f };
	//vertex.normal = { 0.0f, 1.0f, 0.0f };
	//vertex.tangent = { 0.0f, 0.0f, 0.0f };
	//vertexData_.push_back(vertex);
	//// 右下
	//vertex.pos = Vector4{ 1, 0.0f, -1.0f, 1.0f };
	//vertex.texcoord = { 1.0f, 1.0f };
	//vertex.normal = { 0.0f, 1.0f, 0.0f };
	//vertex.tangent = { 0.0f, 0.0f, 0.0f };
	//vertexData_.push_back(vertex);

	//std::vector<uint32_t> indices_;
	//indices_.resize(6);
	//indices_[0] = 0;
	//indices_[1] = 1;
	//indices_[2] = 2;
	//indices_[3] = 1;
	//indices_[4] = 3;
	//indices_[5] = 2;

	//// Meshを作成する
	//mesh_ = std::make_unique<Mesh>();
	//mesh_->Init(Engine::GetDevice(), vertexData_, indices_);

	//material_ = std::make_unique<Material>();
	//material_->Init(Engine::GetDevice());
	//// Textureの設定
	//material_->SetUseTexture("rail_plane.png");

	//worldTransform_ = std::make_unique<WorldTransform>();
	//worldTransform_->Init(Engine::GetDevice());

	BaseGameObject::Init();
	SetObject("rail.obj");

	SetIsLighting(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Update() {
	//worldTransform_->Update();
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Rail::Draw() const {
	/*mesh_->Draw(Engine::GetCommandList());
	material_->Draw(Engine::GetCommandList());
	worldTransform_->Draw(Engine::GetCommandList());
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(Engine::GetCommandList(), material_->GetUseTexture(), 3);
	Engine::GetCommandList()->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
	*/
	BaseGameObject::Draw();
}

void Rail::SetBottomVertex(Mesh::VertexData* vertexData) {
	vertexData_[2] = vertexData[0];
	vertexData_[3] = vertexData[1];
}

#ifdef _DEBUG
void Rail::Debug_Gui() {

}
#endif