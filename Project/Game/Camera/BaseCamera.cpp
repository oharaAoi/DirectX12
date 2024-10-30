#include "BaseCamera.h"

BaseCamera::~BaseCamera() {
	Finalize();
}

void BaseCamera::Finalize() {
	target_ = nullptr;
}

void BaseCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0 , 0, 0.0f},
		{0.0f, 0.0f, -10.0f}
	};

	// 行列の生成
	scaleMat_ =transform_.scale.MakeScaleMat();
	rotateMat_ = transform_.rotate.MakeRotateMat();
	translateMat_ = transform_.translate.MakeTranslateMat();

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);
	viewportMatrix_ = Matrix4x4::MakeViewport(0, 0, kWindowWidth_, kWindowHeight_, 0, 1);

	// sprite描画のためのMatrixの初期化
	projectionMatrix2D_ = Matrix4x4::MakeOrthograhic(0.0f, 0.0f, float(kWindowWidth_), float(kWindowHeight_), 0.0f, 100.0f);
	viewMatrix2D_ = Matrix4x4::MakeUnit();
}

void BaseCamera::Update() {
	// 行列の生成
	scaleMat_ = transform_.scale.MakeScaleMat();
	rotateMat_ = transform_.rotate.MakeRotateMat();
	translateMat_ = transform_.translate.MakeTranslateMat();

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);

	view2DMatrix_ = Multiply(Multiply(scaleMat_, Quaternion().MakeMatrix()), translateMat_);
	
	Matrix4x4 matViewProjection = viewMatrix_ * projectionMatrix_;
	vpvpMatrix_ = matViewProjection * viewportMatrix_;
}
