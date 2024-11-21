#include "FallStone.h"

FallStone::FallStone() {
}

FallStone::~FallStone() {
}

void FallStone::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FallStone::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 4.0f, 3.0f, 3.0f });
	transform_->SetTranslaion({ 0.0f, 20.0f, 12.0f });


	// Colliderの生成
	SetMeshCollider("fallStone");
	meshCollider_->SetOwner(this);
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });


	radius_ = 1.0f;
	tag_ = "fallStone";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////


void FallStone::Update() {
	if (isFalling_) {
		float fallPos = transform_->GetTranslation().y - 5.0f * GameTimer::DeltaTime();
		transform_->SetTranslationY(fallPos);
	}
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////


void FallStone::Draw() const {
	BaseGameObject::Draw();
}

void FallStone::OnCollision([[maybe_unused]] Collider* other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void FallStone::Debug_Gui() {
	if (ImGui::TreeNode("FallStone")) {
		ImGui::Begin("FallStone");

		if (ImGui::Button("position reset")) {
			transform_->SetTranslaion({ 0.0f, 20.0f, 12.0f });
		}

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif // _DEBUG

void FallStone::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
}

void FallStone::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void FallStone::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}