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

	Quaternion q1 = Quaternion(2.0f, 3.0f, 4.0f, 1.0f);
	Quaternion q2 = Quaternion(1.0f, 3.0f, 5.0f, 2.0f);

	Quaternion identity = Quaternion();
	Quaternion conj = q1.Conjugate();
	Quaternion inverse = Quaternion::Inverse(q1);
	Quaternion normal = q1.Normalize();

	Quaternion mul1 = q1 * q2;
	Quaternion mul2 = q2 * q1;

	float norm =q1.Norm();

	DrawQuaternion(identity, "identity");
	DrawQuaternion(conj, "conj");
	DrawQuaternion(inverse, "inverse");
	DrawQuaternion(normal, "normal");
	DrawQuaternion(mul1, "mul1");
	DrawQuaternion(mul2, "mul2");

	ImGui::Text("norm : %f", norm);

	ImGui::End();
}
#endif
