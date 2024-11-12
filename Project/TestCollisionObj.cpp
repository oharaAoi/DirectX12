#include "TestCollisionObj.h"
#include "Game/GameObject/Player.h"

TestCollisionObj::TestCollisionObj() {
}

TestCollisionObj::~TestCollisionObj() {
}

void TestCollisionObj::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 4.0f, 4.0f, 0.0f });

	radius_ = 1.0f;
	tag_ = "hook";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Update() {

	if (isfollowWire_) {
		Vector3 position = player_->GetWireTip()->GetTransform()->GetTranslation();
		position -= player_->GetTransform()->GetTranslation();
		transform_->SetTranslaion(position);
		if (player_->GetThrow()) {
			tag_ = "throwObj";
			Matrix4x4* parentMat = nullptr;
			transform_->SetParent(*parentMat);
			isfollowWire_ = false;
			player_->SetThrow(false);
			velocity_ = player_->GetThrowVelo();
			Vector3 position = transform_->GetTranslation();
			position += player_->GetTransform()->GetTranslation();
			transform_->SetTranslaion(position);
		}
	}
	else {
		Vector3 position = transform_->GetTranslation();
		position += velocity_ * GameTimer::DeltaTime();
		transform_->SetTranslaion(position);
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestCollisionObj::Draw() const {
	BaseGameObject::Draw();
}

void TestCollisionObj::OnCollision(Collider* other) {

	if (other->GetTag() == "wireTip") {
		if (tag_=="canCatchObj") {
			transform_->SetParent(player_->GetTransform()->GetWorldMatrix());
			Vector3 position = transform_->GetTranslation() - player_->GetTransform()->GetTranslation();
			transform_->SetTranslaion(position);
			isfollowWire_ = true;
		}
		if (tag_ == "canPullObj") {

		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TestCollisionObj::Debug_Gui() {
	if (ImGui::TreeNode("TestCollisionObj")) {
		ImGui::Begin("TestCollisionObj");



		ImGui::End();
		ImGui::TreePop();
	}
}
#endif