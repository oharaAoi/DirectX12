#include "Player.h"
#include "Game/Scene/GameScene.h"


Player::Player() {}
Player::~Player() {}

void Player::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("Player.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, 0.0f, 0.0f });

	wire_ = std::make_unique<ClutchWire>();
	wire_->Init();
	wire_->GetTransform()->SetParentTranslation(transform_->GetTranslation());

	wireTip_ = std::make_unique<WireTip>();
	wireTip_->Init();

	// Colliderの生成
	SetMeshCollider("player");
	meshCollider_->SetOwner(this);
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {

	Move();
	Clutch();

	CatchObjectFollow();

	BaseGameObject::Update();

	wire_->Update();
	wireTip_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
	wire_->Draw();
	wireTip_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		
		if (playerState == int(PlayerState::Attack)) {
			isKnockBack_ = true;
		}

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　跳ね返り処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::KnockBack() {

	if (knockBackTime_ == 0.0f) {
		velocity_.y = 10.0f;
	}

	velocity_.x = knockBackSpeed_ * (float)knockBack_LorR_;

	if (knockBackTime_ <= 0.8f) {
		knockBackTime_ += GameTimer::DeltaTime();
	}
	else {
		knockBackTime_ = 0.0f;
		isKnockBack_ = false;
	}

}

void Player::SetInverMatrix(const Matrix4x4& inver) {
	inverMat_ = inver;
}

void Player::SetCameraZDis(float z) {
	camerazDis_ = z;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui() {
	if (ImGui::TreeNode("player")) {
		ImGui::Begin("player");
		if (ImGui::TreeNode("Move")) {

			ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f, 0.0f, 20.0f);
			Vector3 pos = transform_->GetTranslation();
			ImGui::DragFloat3("Trans", &pos.x);

			ImGui::TreePop();
		}

		ShowEasingDebug(easingIndex_);

		ImGui::End();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Wire")) {
		ImGui::DragFloat2("Wire", &clutchEnd_.x);
		ImGui::TreePop();
	}
}
void Player::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}
#endif

Vector3 Player::GetThrowVelo() const {

	Vector3 theow = ScreenToWorldCoordinate(Input::GetMousePosition(), inverMat_, -camerazDis_);
	theow -= transform_->GetTranslation();
	theow = theow.Normalize();

	return theow;
}


void Player::Move() {

	Vector3 pos = transform_->GetTranslation();
	if (pos.y > groundLine_) {
		velocity_.y += gravity_ * GameTimer::DeltaTime();
	}
	Vector3 testPrePos = pos + velocity_ * GameTimer::DeltaTime();
	if (testPrePos.y <= groundLine_) {
		pos.y = groundLine_;
	}
	if (pos.y <= groundLine_) {
		velocity_.y = 0.0f;
		pos.y = groundLine_;
	}

	velocity_.x = 0.0f;
	if (!isSnagged_) {
		if (!isStretching_) {
			float weight = 1.0f;
			if (isPull_ && wireTip_->GetPull()) {
				weight = wireTip_->GetWeight();
			}

			if (Input::IsPressKey(DIK_A)) {
				velocity_.x -= moveSpeed_;
				targetRotate = leftRotate;
				
			}
			if (Input::IsPressKey(DIK_D)) {
				velocity_.x += moveSpeed_;
				targetRotate = rightRotate;

			}


			if (velocity_.x < 0) {

				Vector2 sab = clutchEnd_ - Vector2{ pos.x,pos.y };
				if (sab.x > 0) {
					velocity_ *= weight;
				}
			} else if (velocity_.x > 0) {

				Vector2 sab = clutchEnd_ - Vector2{ pos.x,pos.y };
				if (sab.x < 0) {
					velocity_ *= weight;
				}
			}

		}

		if (isKnockBack_) {
			KnockBack();
		}

		playerState = int(PlayerState::Default);

	} else if (wireTip_->GetSnagged() && isStretchClutch_) {
		playerState = int(PlayerState::Attack);

		velocity_.y = 0.0f;
		clutchLerpTime_ += GameTimer::DeltaTime();
		pos = Lerp(pos, { clutchEnd_.x,clutchEnd_.y,pos.z }, CallEasingFunc(easingIndex_, powf(clutchLerpTime_, 2.0f)));

		float errorLength = (clutchEnd_ - Vector2{ pos.x,pos.y }).Length();
		if (errorLength < 0.1f) {
			isStretchClutch_ = false;
			isSnagged_ = false;
			clutchLerpTime_ = 0.0f;
			playerState = int(PlayerState::Default);
			wireTip_->SetSnagged(false);
		}
	}


	nowRotate = LerpShortAngle(nowRotate, targetRotate, 0.1f);
	transform_->SetQuaternion(Quaternion::AngleAxis(nowRotate, Vector3::UP()));

	pos += velocity_ * GameTimer::DeltaTime();
	if (isPull_ && wireTip_->GetPull()) {
		float length = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Length();
		if (length >= maxClutchLength_) {

			if (velocity_.x < 0) {

				Vector2 sab = clutchEnd_ - Vector2{ pos.x,pos.y };
				if (sab.x > 0) {
					clutchEnd_.x += velocity_.x * GameTimer::DeltaTime();
				}
			}
			else if (velocity_.x > 0) {

				Vector2 sab = clutchEnd_ - Vector2{ pos.x,pos.y };
				if (sab.x < 0) {
					clutchEnd_.x += velocity_.x * GameTimer::DeltaTime();
				}
			}

		}
	}
	transform_->SetTranslaion(pos);
}

void Player::Clutch() {

	if (!Input::IsPressMouse(0)) {
		isRekey_ = true;
		isPull_ = false;
	}

	if (!isKnockBack_) {
		BaseClutch();
	}

	Vector2 wireTipPos = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Normalize() * wire_->GetTransform()->GetScale().y;
	wireTipPos += {transform_->GetTranslation().x, transform_->GetTranslation().y};
	wireTip_->GetTransform()->SetTranslaion({ wireTipPos.x,wireTipPos.y, transform_->GetTranslation().z });
	if (wireTip_->GetHit() && isStretchClutch_) {
		if (isStretching_) {
			clutchEnd_ = wireTipPos;
			if (wireTip_->GetPull()) {
				maxClutchLength_ = wire_->GetTransform()->GetScale().y;
				isSnagged_ = false;
				wireTip_->SetSnagged(false);
			}
		}
		if (wireTip_->GetSnagged()) {
			isSnagged_ = true;
		}
		isStretching_ = false;
	}


	Stretching();


	if (isStretchClutch_) {

		if (!Input::IsPressMouse(0) && !isStretching_) {
			if (!wireTip_->GetSnagged()) {// 移動地点に到達するまで離れない
				isStretchClutch_ = false;
				isReturnClutch_ = true;
			}
		}

		if (!isStretching_) {
			float length = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Length();
			if (length <= maxClutchLength_) {

				Vector3 nowScale = wire_->GetTransform()->GetScale();
				nowScale.y = length;
				wire_->GetTransform()->SetScale(nowScale);

			}

			if (!wireTip_->GetHit()) {
				
				isStretchClutch_ = false;
				isSnagged_ = false;
				clutchLerpTime_ = 0.0f;
				wireTip_->SetSnagged(false);
				isReturnClutch_ = true;
				
			}

			if (wireTip_->GetCautch()) {
				isStretchClutch_ = false;
				isReturnClutch_ = true;
				isSnagged_ = false;
			}

		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　捕まえたオブジェクトを追従させる処理/投げる処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CatchObjectFollow() {
	// 捕まえたオブジェクトがない場合は早期リターン
	if (wireTip_->GetCatchObject() == nullptr) {
		return;
	}
	
	// プレイヤーの座標に合わせる
	auto catchObj = wireTip_->GetCatchObject();
	catchObj->GetTransform()->SetTranslaion(transform_->GetTranslation());

	// 投げる処理
	if (Input::IsTriggerMouse(0)) {
		catchObj->ReadyToThrow(GetThrowVelo());
		wireTip_->ReleseCathcObject();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////
void Player::BaseClutch() {

	Vector2 start;
	Vector2 clutchDirection;
	if (!isReturnClutch_) {// 最大まで伸びて、戻る状態じゃない時
		if (Input::IsPressMouse(0)) {
			if (!wireTip_->GetFollow()) {
				isPull_ = true;

				FirstClutch();

				Vector3 screenPos = transform_->GetTranslation();
				start = { screenPos.x,screenPos.y };
				clutchDirection = (clutchEnd_ - start).Normalize();
				float angle = std::atan2f(clutchDirection.x, clutchDirection.y);

				Quaternion moveRotate = Quaternion::AngleAxis(-angle, Vector3::FORWARD());
				Quaternion rotate = wire_->GetTransform()->GetQuaternion();
				wire_->GetTransform()->SetQuaternion(moveRotate);
			}
			else if (isRekey_ && !isStretchClutch_ && wireTip_->GetFollow() && !isThrow_) {
				isThrow_ = true;
				wireTip_->SetFolllow(false);
			}
		}
	} else {
		Vector3 nowScale = wire_->GetTransform()->GetScale();

		nowScale.y = std::lerp(nowScale.y, 0.0f, returnSpeed_);
		if (nowScale.y < 0.05f) {
			nowScale.y = 0.0f;
			isReturnClutch_ = false;
		}
		wire_->GetTransform()->SetScale(nowScale);
		maxClutchLength_ = 7.0f;
	}

}

void Player::FirstClutch() {

	if (!isStretchClutch_ && isRekey_) {
		Vector2 mousePos = Input::GetMousePosition();

		Vector3 end = ScreenToWorldCoordinate(mousePos, inverMat_, -camerazDis_);

		if (end.x > transform_->GetTranslation().x) {
			targetRotate = rightRotate;
		}
		else if (end.x < transform_->GetTranslation().x) {
			targetRotate = leftRotate;
		}

		end -= transform_->GetTranslation();
		end = end.Normalize() * maxClutchLength_;
		end += transform_->GetTranslation();
		clutchEnd_ = { end.x,end.y };
		isStretching_ = true;
		isStretchClutch_ = true;
		isRekey_ = false;
	}

}

void Player::Stretching() {

	if (isStretching_) {

		Vector3 nowScale = wire_->GetTransform()->GetScale();
		if (nowScale.y <= maxClutchLength_) {
			nowScale.y += stretchSpeed_ * GameTimer::DeltaTime();
			wire_->GetTransform()->SetScale(nowScale);
		} else {
			isStretching_ = false;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
		if (playerState == int(PlayerState::Attack)) {
			if (!isKnockBack_) {
				if (other.GetObbCenter().x > transform_->GetTranslation().x) {
					knockBack_LorR_ = -1;
				}
				else {
					knockBack_LorR_ = 1;
				}
			}
			isKnockBack_ = true;
		}
	} else if (other.GetTag() == "missile") {
		if (!isKnockBack_) {
			if (other.GetObbCenter().x > transform_->GetTranslation().x) {
				knockBack_LorR_ = -1;
			} else {
				knockBack_LorR_ = 1;
			}
		}
		isKnockBack_ = true;
	}
}

void Player::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {

		
	}
}

void Player::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}
