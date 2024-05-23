#include "Camera.h"

Camera::Camera() {
	Init();
}

Camera::~Camera() {
}

void Camera::Init() {
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -6.0f} };

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
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	viewMatrix2D_ = MakeIdentity4x4();
}

void Camera::Draw() {
}
