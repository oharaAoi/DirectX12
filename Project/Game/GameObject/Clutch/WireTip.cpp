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
	tag_ = "notCatchWireTip";

	
	// Colliderの生成
	SetMeshCollider(notCatchStateTag_);
	meshCollider_->SetOwner(this);
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

	meshCollider_->Update(transform_.get(), Vector3{ 1.0f,1.0f,1.0f });
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
	if (!isNeglect_) {
		if (other->GetTag() == "canPullObj") {
			isPull_ = true;
			weight_ = 0.5f;
		}

		if (other->GetTag() == "hook" && !isPull_) {
			isSnagged_ = true;
		}
		if (other->GetTag() == "canCatchObj" && !isPull_) {
			if (!isFollow_) {
				isCautch_ = true;
				isFollow_ = true;
			}
		}
	}
}

void WireTip::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "boss_core" && !isPull_) {
		isHit_ = true;
		if (!isNeglect_) {
			isSnagged_ = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void WireTip::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		isHit_ = true;
		if (!isNeglect_ && !isPull_) {
			isSnagged_ = true;
		}

	} else if (other.GetTag() == "missile" && !isPull_) {
		if (!isCautchObject_) {
			isCautchObject_ = true;

			catchObject_ = dynamic_cast<CanThrowObject*>(other.GetOwner());
			meshCollider_->SetTag(catchStateTag_);

			// この時点でミサイルのタグをフラグを投げる用にする
			catchObject_->SetIsWireCaught(true);
			catchObject_->GetMeshCollider()->SetTag("throwMissile");
			catchObject_->GetTransform()->SetQuaternion(Quaternion());
		}
	}
}

void WireTip::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		isHit_ = true;
		if (!isNeglect_ && !isPull_) {
			isSnagged_ = true;
		}

	} else if (other.GetTag() == "right_hand" && !isPull_ || other.GetTag() == "left_hand" && !isPull_) {
		isHit_ = true;
		isSnagged_ = true;

		isBossAttack_ = true;
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