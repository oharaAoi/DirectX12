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

	/*Quaternion rotation = Quaternion::AngleAxis(0.45f, Vector3{1.0f, 0.4f, -0.2f}.Normalize());
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMat = rotation.MakeMatrix();
	Vector3 rotateByQuaternion = rotation * pointY;
	Vector3 rotateByMatrix = Transform(pointY, rotateMat);

	DrawQuaternion(rotation, "rotation");
	DrawMatrix(rotateMat, "rotateMatrix");
	DrawVector(rotateByQuaternion, "rotateByQuaternion");
	DrawVector(rotateByMatrix, "rotateByMatrix");*/

	Quaternion rotate0 = Quaternion::AngleAxis(0.3f, {0.71f, 0.71f, 0.0f});
	//Quaternion rotate1 = Quaternion::AngleAxis(PI, {0.71f, 0.0f, 0.71f});
	Quaternion rotate1 = Quaternion(-rotate0.x, -rotate0.y, -rotate0.z, -rotate0.w);
	
	Quaternion lerp0 = Quaternion::Slerp(rotate0, rotate1, 0.0f);
	Quaternion lerp1 = Quaternion::Slerp(rotate0, rotate1, 0.3f);
	Quaternion lerp2 = Quaternion::Slerp(rotate0, rotate1, 0.5f);
	Quaternion lerp3 = Quaternion::Slerp(rotate0, rotate1, 0.7f);
	Quaternion lerp4 = Quaternion::Slerp(rotate0, rotate1, 1.0f);

	DrawQuaternion(lerp0, " : Slerp(0.0f)");
	DrawQuaternion(lerp1, " : Slerp(0.3f)");
	DrawQuaternion(lerp2, " : Slerp(0.5f)");
	DrawQuaternion(lerp3, " : Slerp(0.7f)");
	DrawQuaternion(lerp4, " : Slerp(1.0f)");
	
	ImGui::End();
}
#endif
