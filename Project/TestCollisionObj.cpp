#include "TestCollisionObj.h"

TestCollisionObj::TestCollisionObj() {
}

TestCollisionObj::~TestCollisionObj() {
}

void TestCollisionObj::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 4.0f, 4.0f, 0.0f });

	radius_ = 1.0f;
	tag_ = "hook";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Update() {
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Draw() const {
	BaseGameObject::Draw();
}

void TestCollisionObj::OnCollision(Collider* other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TestCollisionObj::Debug_Gui() {
	if (ImGui::TreeNode("TestCollisionObj")) {
		ImGui::Begin("TestCollisionObj");



		ImGui::End();
		ImGui::TreePop();
	}
}
#endif