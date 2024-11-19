#include "Fall.h"

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
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 8.0f, 9.0f, 0.0f });

	radius_ = 1.0f;
	tag_ = "fallObj";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Fall::Update() {

	

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Fall::Draw() const {

	BaseGameObject::Draw();
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Fall::Debug_Gui() {
	if (ImGui::TreeNode("TestCollisionObj")) {
		ImGui::Begin("TestCollisionObj");



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