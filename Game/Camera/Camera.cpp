#include "Camera.h"

Camera::Camera() {
	Init();
}

Camera::~Camera() {
}

void Camera::Init() {
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.3f, 0.0f, 0.0f}, {0.0f, 2.0f, -6.0f} };

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

	debugCameraMode_ = true;
	lookPosition_ = { 0,0,0 };
	mousePosition_ = { 0,0 };
}

void Camera::Update() {
	// デバックカメラモードの時
	if (debugCameraMode_) {
		ScrollMove();
		TransitionMove();
		RotateMove();
	}

	scaleMat_ = MakeScaleMatrix(transform_.scale);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	viewMatrix2D_ = MakeIdentity4x4();

	ImGui::Begin("Camera");
	if (ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -6.0f} };
		rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	}
	ImGui::End();
}

void Camera::Draw() {
}

/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
void Camera::TransitionMove() {
	if (Input::IsPressMouse(2)) {
		Vector2 pos = Input::GetMousePosition();

		Vector2 diff{};
		diff.x = static_cast<float>(pos.x - mousePosition_.x);
		diff.y = static_cast<float>(pos.y - mousePosition_.y);

		const float speed = 0.01f;
		Vector3 move{};
		// カメラの向きを求める
		move = TransformNormal(Normalize({ diff.x, diff.y, 0 }) * speed, cameraMatrix_);

		transform_.translate += move;
		lookPosition_ += move;

	} else if (!Input::IsPressMouse(2)) {
		mousePosition_ = Input::GetMousePosition();
	}
}

/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
void Camera::RotateMove() {
	if (Input::IsPressMouse(1)) {
		Vector3 normalizeDiff{};
		Vector3 offset = { 0.0f, 2.0f, -6.0f };
		const float speed = 0.015f;
		// マウスの位置を得る
		Vector2 mousePos = Input::GetMousePosition();
		// マウスの移動量を求める
		Vector2 rotateDiff{};
		rotateDiff.x = static_cast<float>(mousePos.x - rotateMousePosition_.x);
		rotateDiff.y = static_cast<float>(mousePos.y - rotateMousePosition_.y);
		// 正規化する
		normalizeDiff = Normalize({ rotateDiff.x , rotateDiff.y, 0 }) * speed;

		// 追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta *= MakeRotateXMatrix(normalizeDiff.y);
		matRotDelta *= MakeRotateYMatrix(normalizeDiff.x);

		// 累積の回転行列の合成
		rotateMat_ = matRotDelta * rotateMat_;

		/// -------------------------------------------------------------------------
		// 旋回させる
		offset = TransformNormal(Vector3(0, 0, -5.798f), rotateMat_);
		// 位置を動かす
		transform_.translate = lookPosition_ + offset;

		cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), MakeTranslateMatrix(transform_.translate));
		viewMatrix_ = Inverse(cameraMatrix_);

	} else if (!Input::IsPressMouse(1)) {
		rotateMousePosition_ = Input::GetMousePosition();
	}
}

/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
void Camera::ScrollMove() {
	if (Input::GetWheel() > 0) {
		Vector3 direction = { 0,0,1 };

		direction = TransformNormal(direction, rotateMat_);
		Normalize(direction);

		direction *= kCameraMoveSpeed_;

		transform_.translate += direction;
		lookPosition_ += direction;


	} else if (Input::GetWheel() < 0) {
		Vector3 direction = { 0,0,-1 };

		direction = TransformNormal(direction, rotateMat_);
		Normalize(direction);

		direction *= kCameraMoveSpeed_;

		transform_.translate += direction;
		lookPosition_ += direction;
	}
}
