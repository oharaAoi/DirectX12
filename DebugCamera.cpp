#include "DebugCamera.h"

DebugCamera::DebugCamera() { Init(); }
DebugCamera::~DebugCamera() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void DebugCamera::Init() {
	transform_ = {
	{1.0f, 1.0f, 1.0f},
	{0 , 0, 0.0f},
	{0.0f, 0.0f, -10.0f}
	};
	
	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	debugCameraMode_ = true;
	lookPosition_ = { 0,0,0 };
	mousePosition_ = { 0,0 };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void DebugCamera::Update() {
	ScrollMove();
	TransitionMove();
	RotateMove();

	scaleMat_ = MakeScaleMatrix(transform_.scale);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Draw() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラの移動
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::TransitionMove() {
	if (Input::IsPressMouse(2)) {
		Vector2 pos = Input::GetMousePosition();

		Vector2 diff{};
		diff.x = static_cast<float>(pos.x - mousePosition_.x);
		diff.y = static_cast<float>(pos.y - mousePosition_.y);

		const float speed = 0.04f;
		Vector3 move{};
		// カメラの向きを求める
		move = TransformNormal(Normalize({ diff.x, diff.y, 0 }) * speed, cameraMatrix_);

		transform_.translate += move;
		lookPosition_ += move;

	} else if (!Input::IsPressMouse(2)) {
		mousePosition_ = Input::GetMousePosition();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラの回転
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::RotateMove() {
	if (Input::IsPressMouse(1)) {
		Vector3 normalizeDiff{};
		Vector3 offset = { 0.0f, 0.0f, -10.0f };
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
		offset = TransformNormal(Vector3(0, 0, -10.0f), rotateMat_);
		// 位置を動かす
		transform_.translate = lookPosition_ + offset;

		cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), MakeTranslateMatrix(transform_.translate));
		viewMatrix_ = Inverse(cameraMatrix_);

	} else if (!Input::IsPressMouse(1)) {
		rotateMousePosition_ = Input::GetMousePosition();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラの奥行を変更
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::ScrollMove() {
	if (Input::GetWheel() > 0) {
		Vector3 direction = { 0,0,1 };

		direction = TransformNormal(direction, rotateMat_);
		Normalize(direction);

		direction *= kDebugCameraMoveSpeed_;

		transform_.translate += direction;
		lookPosition_ += direction;


	} else if (Input::GetWheel() < 0) {
		Vector3 direction = { 0,0,-1 };

		direction = TransformNormal(direction, rotateMat_);
		Normalize(direction);

		direction *= kDebugCameraMoveSpeed_;

		transform_.translate += direction;
		lookPosition_ += direction;
	}
}
