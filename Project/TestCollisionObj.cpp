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
	transform_->SetTranslaion({ 4.0f, 7.0f, 0.0f });

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
			Vector3* parentVec = nullptr;
			transform_->SetParentTranslation(*parentVec);
			isfollowWire_ = false;
			player_->SetThrow(false);
			velocity_ = player_->GetThrowVelo();
			Vector3 pos = transform_->GetTranslation();
			pos += player_->GetTransform()->GetTranslation();
			transform_->SetTranslaion(pos);
		}
	}
	else {
		Vector3 position = transform_->GetTranslation();
		position += velocity_ * GameTimer::DeltaTime();
		transform_->SetTranslaion(position);
	}

	if (isPulling_&& player_->GetPull()) {
		Vector3 position = player_->GetWireTip()->GetTransform()->GetTranslation();
		transform_->SetTranslaion({ position.x,transform_->GetTranslation().y,position.z });
	}
	if (!player_->GetPull()) {
		isPulling_ = false;
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
	if (other->GetTag() == "notCatchWireTip") {
		if (!player_->GetWireTip()->GetNeglect()) {
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