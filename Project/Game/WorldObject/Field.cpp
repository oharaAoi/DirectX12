#include "Field.h"

Field::Field() {}
Field::~Field() {}

void Field::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////


void Field::Init() {
	BaseGameObject::Init();
	SetObject("kair_field.obj");
	SetColor(Vector4(0.65f, 0.16f, 0.16f, 1.0f));

	SetMeshCollider("field");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Field::Update() {
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Field::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定系
////////////////////////////////////////////////////////////////////////////////////////////
void Field::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
}
void Field::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}
void Field::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

#ifdef _DEBUG
void Field::Debug_Gui() {
	if (ImGui::TreeNode("Field")) {
		ImGui::Begin("Field");
		transform_->Debug_Gui();
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif