#include "TestScene.h"
#include "Engine/Utilities/AdjustmentItem.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
}

void TestScene::Draw() const {
}

#ifdef _DEBUG
void TestScene::Debug_Gui() {
	ImGui::Begin("TestScene");

	Vector3 from0 = Vector3(1.0f, 0.7f, 0.5f).Normalize();
	Vector3 to0 = -1 * from0;
	Vector3 from1 = Vector3(-0.6f, 0.9f, 0.2f).Normalize();
	Vector3 to1 = Vector3(0.4f, 0.7f, -0.5f).Normalize();

	Quaternion rotate2 = Quaternion::FromToRotation(from0, to0);
	Quaternion rotate1 = Quaternion::FromToRotation(Vector3(1.0f, 0.0f, 0.0f).Normalize(), Vector3(-1.0f, 0.0f, 0.0f).Normalize());
	Quaternion rotate3 = Quaternion::FromToRotation(from1, to1);

	DrawMatrix(rotate1.MakeMatrix(), "matrix1");
	DrawMatrix(rotate2.MakeMatrix(), "matrix2");
	DrawMatrix(rotate3.MakeMatrix(), "matrix3");

	ImGui::End();
}
#endif
