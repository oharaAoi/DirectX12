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

	
	// Colliderの生成
	SetMeshCollider("wireTip");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::Update() {
	isHit_ = false;
	isCautch_ = false;
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


	} else if(other->GetTag() == "canCatchObj") {
		if (!isFollow_) {
			isCautch_ = true;
			isFollow_ = true;
		}


	} else if(other->GetTag() == "canPullObj") {
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
// ↓　当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		isHit_ = true;
		isSnagged_ = true;

	} else if (other.GetTag() == "missile") {
		if (!isCautchObject_) {
			isCautchObject_ = true;
			meshCollider_->SetIsnotCheckCollision(true);
		}
		// 衝突相手のポインタを保存(一番最初の衝突のみ)
		if (pCollisionMesh_ == nullptr) {
			pCollisionMesh_ = &other;
		}
	}
}

void WireTip::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		isHit_ = true;
		isSnagged_ = true;
	}
}

void WireTip::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
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