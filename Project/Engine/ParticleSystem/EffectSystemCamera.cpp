#include "EffectSystemCamera.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif // _DEBUG

EffectSystemCamera::EffectSystemCamera() { Init(); }
EffectSystemCamera::~EffectSystemCamera() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.0f, 0.0f},
		{0.0f, 5.0f, -9.0f}
	};

	scaleMat_ = transform_.scale.MakeScaleMat();
	rotateMat_ = transform_.rotate.MakeRotateMat();
	translateMat_ = transform_.translate.MakeTranslateMat();

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemCamera::Update() {
	scaleMat_ = transform_.scale.MakeScaleMat();
	rotateMat_ = transform_.rotate.MakeRotateMat();
	translateMat_ = transform_.translate.MakeTranslateMat();

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);

#ifdef _DEBUG
	ImGui::Begin("effectCamera");
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
#endif
}
