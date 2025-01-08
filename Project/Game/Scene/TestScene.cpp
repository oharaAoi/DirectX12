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

	Quaternion rotation = Quaternion::AngleAxis(0.45f, Vector3{1.0f, 0.4f, -0.2f}.Normalize());
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMat = rotation.MakeMatrix();
	Vector3 rotateByQuaternion = rotation * pointY;
	Vector3 rotateByMatrix = Transform(pointY, rotateMat);

	DrawQuaternion(rotation, "rotation");
	DrawMatrix(rotateMat, "rotateMatrix");
	DrawVector(rotateByQuaternion, "rotateByQuaternion");
	DrawVector(rotateByMatrix, "rotateByMatrix");
	

	
	ImGui::End();
}
#endif
