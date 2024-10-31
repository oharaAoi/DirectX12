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