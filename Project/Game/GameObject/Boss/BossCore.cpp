#include "BossCore.h"
#include "Game/GameObject/Boss/BossCoreDefaultState.h"
#include "Game/GameObject/Boss/BossCoreHideState.h"
#include "Game/GameObject/Boss/BossCoreAppearState.h"
#include "Game/GameObject/Boss/BossCoreGameStartState.h"
#include "Engine/Utilities/AdjustmentItem.h"

BossCore::BossCore() {}
BossCore::~BossCore() {}

void BossCore::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Init() {
	BaseGameObject::Init();
	SetObject("boss_core.obj");

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	// colliderの設定
	SetMeshCollider("boss_core");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	// 調整項目の適応
	AdaptAdjustment();


	state_ = std::make_unique<BossCoreDefaultState>(this);
	behaviorRequest_ = CoreState::GameStart;


	defaultPosition_ = transform_->GetTranslation();
	middlePosition_ = { 0.0f,13.0f,-9.0f };
	endPosition_ = { 0.0f,7.0f, - 18.0f };

	hp_ = 100.0f;

	isFirstHit_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Update() {

	CheckRequest();

	if (isNear_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		if (Input::IsPressKey(DIK_A) || Input::IsPressKey(DIK_D)) {
			energy_ += 10.0f * GameTimer::DeltaTime();
		}
		if (energy_ >= canFallEnergy) {
			isFalsePullBack_ = true;
			isFalling_ = true;
			behaviorRequest_ = CoreState::Appear;
		}

	}
	else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	state_->Update();

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

void BossCore::CheckRequest() {
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case CoreState::Default:
			SetBehaviorState(std::make_unique<BossCoreDefaultState>(this));

			break;
		case CoreState::Hide:
			SetBehaviorState(std::make_unique<BossCoreHideState>(this));
			break;
		case CoreState::Appear:
			SetBehaviorState(std::make_unique<BossCoreAppearState>(this));
			break;
		case CoreState::GameStart:
			SetBehaviorState(std::make_unique<BossCoreGameStartState>(this));
			break;
		default:
			break;
		}

		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void BossCore::ChangeHide() {
	isFalling_ = false;
	energy_ = 0.0f;
	behaviorRequest_ = CoreState::Hide;
}

bool BossCore::CheckMouseNear(const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	// objectのscreen座標を求める
	Vector3 objectScreen = Transform(Vector3::ZERO(), transform_->GetWorldMatrix() * vpvpMat);
	Vector2 objectScreenPos_ = Vector2(objectScreen.x, objectScreen.y);

	if (isFalling_) {
		isNear_ = false;
		return isNear_;
	}

	if (isPlayerPullBack_) {
		if (isNear_) {
			return isNear_;
		}
	}

	isNear_ = false;
	// 長さを取って距離が近かったら
	if ((mousePos - objectScreenPos_).Length() < 40.0f) {
		isNear_ = true;
	}
	return isNear_;

}

bool BossCore::SetFalsePlayerPullBack() {
	bool is = isFalsePullBack_;
	isFalsePullBack_ = false;
	return is;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (behavior_ != CoreState::GameStart) {
		if (other.GetTag() == "player") {
			hp_ -= decrementHp_;
		}
	} else {
		if (other.GetTag() == "player") {
			isFirstHit_ = true;
		}
	}
}


void BossCore::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}
void BossCore::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossCore::Debug_Gui() {
	ImGui::Begin("Boss_Core");
	transform_->Debug_Gui();

	ImGui::Text("Energy:%f", energy_);
	if (ImGui::Button("energy reset")) {
		energy_ = 0.0f;
		isFalling_ = false;
	}
	if (ImGui::Button("position reset")) {
		transform_->SetTranslationZ(defaultPosition_.z);
	}

	ImGui::SliderFloat("hp", &hp_, 0.0f, 100.0f);

	ImGui::End();
}

void BossCore::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}
#endif