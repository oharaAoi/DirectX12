#include "BossEye.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"

BossEye::BossEye() {
}

BossEye::~BossEye() {
}

void BossEye::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEye::Init() {
	BaseGameObject::Init();
	SetObject("boss_eye.obj");
	SetIsLighting(false);

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	AdaptAdjustment();

	alpha_ = 0.0f;
	shineTime_ = 0.0f;
	shineTimeLimit_ = 1.0f;

	SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha_));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEye::Update() {
	if (isShine_) {
		Shine();
	}
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEye::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEye::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　目を光らせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEye::Shine() {
	shineTime_ += GameTimer::DeltaTime();
	float t = shineTime_ / shineTimeLimit_;

	alpha_ = std::lerp(0.0f, 1.0f, CallEasingFunc(easingType_, t));

	if (shineTime_ >= shineTimeLimit_) {
		isShine_ = false;
	}

	SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha_));
}

void BossEye::SetShine() {
	isShine_ = true;
	shineTime_ = 0.0f;
}

#ifdef _DEBUG
void BossEye::Debug_Gui() {
	ImGui::Begin("Boss_Eye");
	transform_->Debug_Gui();
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->ImGuiDraw();
	}

	ImGui::SliderFloat("shineTime", &shineTime_, 0.0f, shineTimeLimit_);
	ImGui::DragFloat("shineTimeLimite", &shineTimeLimit_, 0.1f);

	ShowEasingDebug(easingType_);

	if (ImGui::Button("isShine")) {
		isShine_ = true;
		shineTime_ = 0.0f;
	}

	ImGui::End();
}
#endif