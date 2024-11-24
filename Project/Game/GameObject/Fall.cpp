#include "Fall.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Engine.h"

Fall::Fall() {
}

Fall::~Fall() {
}

void Fall::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Fall::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 1.0f, 2.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, 16.0f, 12.0f });

	radius_ = 1.0f;
	tag_ = "fallObj";

	// -------------------------------------------------
	// ↓ Uiの初期化
	// -------------------------------------------------

	fallGuideUI_ = Engine::CreateSprite("kari_fallUI.png");

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Fall::Update() {

	if (isNear_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		if (Input::IsPressKey(DIK_A)||Input::IsPressKey(DIK_D)) {
			energy_ += 10.0f * GameTimer::DeltaTime();
		}
		if (energy_ >= canFallEnergy) {
			player_->SetFalsePullBack();
			isFalling_ = true;
		}

	}
	else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f,1.0f));
	}

	if (isFalling_) {
		velocity_.y += gravity_ * GameTimer::DeltaTime();
		float fallPos = transform_->GetTranslation().y;
		fallPos += velocity_.y * GameTimer::DeltaTime();
		transform_->SetTranslationY(fallPos);
	}

	BaseGameObject::Update();

	Vector3 worldPos = Transform(Vector3::ZERO(), transform_->GetWorldMatrix() * Render::GetVpvpMat());
	Vector2 scrennPos = Vector2(worldPos.x + 100, worldPos.y + 50);
	fallGuideUI_->SetTranslate(scrennPos);
	fallGuideUI_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Fall::Draw() const {

	BaseGameObject::Draw();
}

void Fall::DrawUI() const {
	fallGuideUI_->Draw();
}

void Fall::OnCollision([[maybe_unused]] Collider* other) {
	/*if (other->GetTag() == "wireTip") {
		if (tag_ == "canCatchObj") {
			if (!player_->GetIsKnockBack()) {
				transform_->SetParentTranslation(player_->GetTransform()->GetTranslation());
				Vector3 position = transform_->GetTranslation() - player_->GetTransform()->GetTranslation();
				transform_->SetTranslaion(position);
				isfollowWire_ = true;
			}
			else {
				player_->GetWireTip()->SetFolllow(false);
			}

		}

		if (tag_ == "canPullObj") {

			isPulling_ = true;
		}
	}*/

}

bool Fall::CheckMouseNear(const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	// objectのscreen座標を求める
	Vector3 objectScreen = Transform(Vector3::ZERO(), transform_->GetWorldMatrix() * vpvpMat);
	Vector2 objectScreenPos_ = Vector2(objectScreen.x, objectScreen.y);

	if (player_->GetPullBack()) {
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

void Fall::Reset() {
	velocity_.y = 0.0f;
	transform_->SetTranslaion({ 0.0f, 16.0f, 12.0f });
	isFalling_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Fall::Debug_Gui() {
	if (ImGui::TreeNode("Fall")) {
		ImGui::Begin("Fall");

		ImGui::Text("Energy:%f", energy_);
		if (ImGui::Button("energy reset")) {
			energy_ = 0.0f;
			isFalling_ = false;
		}
		if (ImGui::Button("position reset")) {
			transform_->SetTranslaion({ 0.0f, 12.0f, 0.0f });
		}

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif


void Fall::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {

}
void Fall::OnCollisionStay([[maybe_unused]] MeshCollider& other) {

}
void Fall::OnCollisionExit([[maybe_unused]] MeshCollider& other) {

}