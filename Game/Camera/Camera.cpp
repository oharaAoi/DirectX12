#include "Camera.h"

Camera::Camera() {
	Init();
}

Camera::~Camera() {
}

void Camera::Init() {
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.5f, 0.0f, 0.0f}, {0.0f, 5.0f, -9.0f} };
	
	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	// sprite描画のためのMatrixの初期化
	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	viewMatrix2D_ = MakeIdentity4x4();
}

void Camera::Update() {

	// デバックカメラモードの時
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.5f, 0.0f, 0.0f}, {0.0f, 5.0f, -9.0f} };
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);


	scaleMat_ = MakeScaleMatrix(transform_.scale);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	viewMatrix2D_ = MakeIdentity4x4();

	ImGui::Begin("Camera");
	if (ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -9.0f} };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	}

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);

	ImGui::End();
}

void Camera::Draw() {
}