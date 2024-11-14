#include "WireTip.h"

WireTip::WireTip() {
}

WireTip::~WireTip() {
}

void WireTip::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 0.25f, 0.25f, 0.25f });
	transform_->SetTranslaion({ 0.0f, 0.0f, 0.0f });
	tag_ = "wireTip";

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));
	meshCollider_->SetTag("wireTip");
	meshCollider_->SetOnCollisionCallBack([this](MeshCollider& other) {
		this->OnCollision(other);
										  });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::Update() {
	isHit_ = false;
	isCautch_ = false;
	isSnagged_ = false;
	isPull_ = false;
	weight_ = 0.0f;
	BaseGameObject::Update();

	meshCollider_->Update(transform_.get(), Vector3::ZERO());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::Draw() const {
	BaseGameObject::Draw();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::OnCollision(Collider* other) {

	isHit_ = true;

	if (other->GetTag() == "hook") {
		isSnagged_ = true;
	}

	if (other->GetTag() == "canCatchObj") {
		if (!isFollow_) {
			isCautch_ = true;
			isFollow_ = true;
		}
	}

	if (other->GetTag() == "canPullObj") {
		isPull_ = true;
		weight_ = 0.5f;
	}

}

void WireTip::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		isHit_ = true;
		isSnagged_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void WireTip::Debug_Gui() {
	if (ImGui::TreeNode("WireTip")) {
		ImGui::Begin("WireTip");



		ImGui::End();
		ImGui::TreePop();
	}
}
#endif