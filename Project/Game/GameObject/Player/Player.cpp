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
	SetObject("Player.gltf");
	SetAnimater("./Game/Resources/Model/Player/", "Player.gltf", true, true, true);
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, 0.0f, 0.0f });

	wire_ = std::make_unique<ClutchWire>();
	wire_->Init();
	wire_->GetTransform()->SetParentTranslation(transform_->GetTranslation());

	wireTip_ = std::make_unique<WireTip>();
	wireTip_->Init();

	// animatorの生成
	playerAnimator_ = std::make_unique<PlayerAnimator>(this);
	
	// Colliderの生成
	SetMeshCollider("player");
	meshCollider_->SetOwner(this);
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	// 状態の定義
	state_ = std::make_unique<PlayerRootState>(this);
	behaviorRequest_ = PlayerState::Default;

	// -------------------------------------------------
	// ↓ 調整項目の適応
	// -------------------------------------------------
	adjustItem_ = AdjustmentItem::GetInstance();
	adjustItem_->AddItem(groupName_, "pos", transform_->GetTranslation());

	AdaptAdjustment();

	// -------------------------------------------------
	// ↓ 変数の初期化
	// -------------------------------------------------
	canBossAttack_ = false;

	throwSpeed_ = 6.0f;

	playerAnimator_->NowToAfterTransition("defalut");

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {

	CheckBehaviorRequest();
	state_->Update();

	if (!isAutoMove_) {
		Move();
	}
	Clutch();

	if (!isStretchClutch_&&!isReturnClutch_) {
		wireTip_->SetIsBossAttack(false);
	}

	CatchObjectFollow();

	playerAnimator_->Update();

	BaseGameObject::Update();

	wire_->Update();
	wireTip_->Update();

	isAutoMove_ = false;
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
// ↓　状態の遷移
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckBehaviorRequest() {
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case PlayerState::Default:
			SetBehaviorState(std::make_unique<PlayerRootState>(this));
			break;
		case PlayerState::Attack:

			break;
		case PlayerState::BeAttacked:
			SetBehaviorState(std::make_unique<PlayerBeAttackedState>(this));
			break;
		default:
			break;
		}

		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "boss_core") {

		//if (playerState == int(PlayerState::Attack)) {
		//	isKnockBack_ = true;
		//}

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
	if (knockBack_LorR_ < 0) {
		targetRotate = rightRotate;
	} else if (knockBack_LorR_ > 0) {
		targetRotate = leftRotate;
	}

	if (knockBackTime_ <= 0.8f) {
		knockBackTime_ += GameTimer::DeltaTime();
	} else {
		knockBackTime_ = 0.0f;
		isKnockBack_ = false;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Bossとの距離を判定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckBossToLength(const Vector3& bossPos) {
	bossDire_ = bossPos - transform_->GetTranslation();
	bossDire_.y = 2.0f;
	bossDire_ = bossDire_.Normalize();

	float lengthX = std::abs(transform_->GetTranslation().x - bossPos.x);
	if (lengthX < bossAttackRange_) {
		canBossAttack_ = true;
	} else {
		canBossAttack_ = false;
	}

	/*Quaternion rotate = Quaternion::AngleAxis(0.44f, Vector3{1.0f, 1.0f, 1.0f});
	Matrix4x4 mat = rotate.MakeMatrix();*/
}

Vector3 Player::GetThrowVelo() const {

	Vector3 theow = ScreenToWorldCoordinate(Input::GetMousePosition(), inverMat_, -camerazDis_);
	theow -= transform_->GetTranslation();
	theow = theow.Normalize();

	return theow;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　捕まえたオブジェクトを追従させる処理/投げる処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CatchObjectFollow() {
	// 捕まえたオブジェクトがない場合は早期リターン
	if (wireTip_->GetCatchObject() == nullptr || !wireTip_->GetIsCautchObject()) {
		return;
	}

	// プレイヤーの座標に合わせる
	auto catchObj = wireTip_->GetCatchObject();
	Vector3 playerPos = transform_->GetTranslation();
	playerPos.y += 2.0f;
	catchObj->GetTransform()->SetTranslaion(playerPos);
	catchObj->GetTransform()->SetQuaternion(transform_->GetQuaternion());

	// 投げる処理
	if (Input::IsTriggerMouse(0)) {
		if (canBossAttack_) {
			catchObj->ReadyToThrow(bossDire_.Normalize() * throwSpeed_);
			wireTip_->SetIsCautchObject(false);
			wireTip_->ReleseCathcObject();
			isRekey_ = false;
		} else {
			catchObj->ReadyToThrow(GetThrowVelo() * throwSpeed_);
			wireTip_->SetIsCautchObject(false);
			wireTip_->ReleseCathcObject();
			isRekey_ = false;
		}
	}
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

	if (!isPullBackObj_) {
		DefaultMove(pos);
	} else {
		PullBackMove(pos);
	}

	if (groundLine_ >= pos.y && velocity_.x == 0 && canBossAttack_ && behavior_ != PlayerState::Attack) {
		targetRotate = 0.0f;
		playerAnimator_->NowToAfterTransition("defalut");
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
			} else if (velocity_.x > 0) {

				Vector2 sab = clutchEnd_ - Vector2{ pos.x,pos.y };
				if (sab.x < 0) {
					clutchEnd_.x += velocity_.x * GameTimer::DeltaTime();
				}
			}

		}
	}
	transform_->SetTranslaion(pos);
	if (!isPullBackObj_) {
		beforePosition_ = pos;
	}
}

void Player::DefaultMove(Vector3& pos) {

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
				playerAnimator_->NowToAfterTransition("move");

			}
			if (Input::IsPressKey(DIK_D)) {
				velocity_.x += moveSpeed_;
				targetRotate = rightRotate;
				playerAnimator_->NowToAfterTransition("move");
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

		// 速度がないのならキーが押されていないのでdefalutのanimationにする
		if (velocity_.x == 0) {
			playerAnimator_->NowToAfterTransition("defalut");
		}
	} else if (isSnagged_ && wireTip_->GetSnagged() && isStretchClutch_) {
		playerState = int(PlayerState::Attack);
		playerAnimator_->NowToAfterTransition("attack");

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
}

void Player::PullBackMove(Vector3& pos) {
	pos.x = beforePosition_.x;
	if (Input::IsPressKey(DIK_A) || Input::IsPressKey(DIK_D)) {
		pos.x += RandomFloat(-4.0f, 4.0f) * GameTimer::DeltaTime();
	}

}

void Player::Clutch() {

	if (!Input::IsPressMouse(0)) {
		isRekey_ = true;
		isPull_ = false;
	}

	BaseClutch();

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
			if (wireTip_->GetPull()) {
				isSnagged_ = false;
				wireTip_->SetSnagged(false);
			}
		}
		isStretching_ = false;
	}


	if (!isPullBackObj_) {
		Stretching();
	} else {
		BackPullStretch();
	}


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
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////
void Player::BaseClutch() {

	Vector2 start;
	Vector2 clutchDirection;
	if (!isReturnClutch_) {// 最大まで伸びて、戻る状態じゃない時
		if (!isKnockBack_) {
			if (Input::IsPressMouse(0)) {
				if (!wireTip_->GetFollow() && !wireTip_->GetIsCautchObject()) {
					isPull_ = true;

					FirstClutch();

					Vector3 screenPos = transform_->GetTranslation();
					start = { screenPos.x,screenPos.y };
					clutchDirection = (clutchEnd_ - start).Normalize();
					float angle = std::atan2f(clutchDirection.x, clutchDirection.y);

					Quaternion moveRotate = Quaternion::AngleAxis(-angle, Vector3::FORWARD());
					Quaternion rotate = wire_->GetTransform()->GetQuaternion();
					wire_->GetTransform()->SetQuaternion(moveRotate);
				} else if (isRekey_ && !isStretchClutch_ && wireTip_->GetFollow() && !isThrow_) {
					isThrow_ = true;
					wireTip_->SetFolllow(false);
				}
			}
		}
	} else {
		Vector3 nowScale = wire_->GetTransform()->GetScale();

		nowScale.y = std::lerp(nowScale.y, 0.0f, returnSpeed_);
		if (nowScale.y < 0.05f) {
			nowScale.y = 0.0f;
			isReturnClutch_ = false;
			maxClutchLength_ = defaultMaxClutchLength_;
		}
		wire_->GetTransform()->SetScale(nowScale);
		wireTip_->SetNeglect(true);
	}

}

void Player::FirstClutch() {

	if (isNearBack_ && isRekey_) {

		ClutchEndCalculation();
		float length = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Length();
		maxClutchLength_ = length;
		isStretching_ = true;
		isPullBackObj_ = true;
		wireTip_->SetNeglect(true);
		isRekey_ = false;
	} else if (!isStretchClutch_ && isRekey_ && !isPullBackObj_) {
		wireTip_->SetNeglect(false);
		ClutchEndCalculation();
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

void Player::BackPullStretch() {
	if (isStretching_) {

		Vector3 nowScale = wire_->GetTransform()->GetScale();
		if (nowScale.y <= maxClutchLength_) {
			nowScale.y += stretchSpeed_ * GameTimer::DeltaTime();
			wire_->GetTransform()->SetScale(nowScale);
		} else {
			isStretching_ = false;
		}
	} else {
		if (!Input::IsPressMouse(0)) {
			isReturnClutch_ = true;
			isPullBackObj_ = false;
			wireTip_->SetNeglect(false);
		}
	}
}

void Player::ClutchEndCalculation() {
	Vector2 mousePos = Input::GetMousePosition();

	Vector3 end = ScreenToWorldCoordinate(mousePos, inverMat_, -camerazDis_);

	if (end.x > transform_->GetTranslation().x) {
		targetRotate = rightRotate;
	} else if (end.x < transform_->GetTranslation().x) {
		targetRotate = leftRotate;
	}

	if (isNearBack_) {
		clutchEnd_ = { end.x,end.y };
	} else {
		end -= transform_->GetTranslation();
		end = end.Normalize() * maxClutchLength_;
		end += transform_->GetTranslation();
		clutchEnd_ = { end.x,end.y };
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
				} else {
					knockBack_LorR_ = 1;
				}
			}
			isKnockBack_ = true;
		}
	} else if (other.GetTag() == "missile") {
		behaviorRequest_ = PlayerState::BeAttacked;
		beAttackedType_ = BeAttackedType::NORMAL_HITED;
		isReturnClutch_ = true;
		isStretching_ = false;
		isPullBackObj_ = false;
		wireTip_->SetNeglect(false);

	} else if (other.GetTag() == "right_hand" || other.GetTag() == "left_hand") {

		if (wireTip_->GetIsBossAttack()) {
			if (!isKnockBack_) {
				if (other.GetObbCenter().x > transform_->GetTranslation().x) {
					knockBack_LorR_ = -1;
				} else {
					knockBack_LorR_ = 1;
				}
			}
			isKnockBack_ = true;
			isReturnClutch_ = true;
			isStretching_ = false;
			isPullBackObj_ = false;
			wireTip_->SetNeglect(false);
			wireTip_->SetIsBossAttack(false);

		} else {
			if (other.GetSubTag() == "attacked") {
				if (other.GetSubTag() != "wait_hand") {
					behaviorRequest_ = PlayerState::BeAttacked;
					beAttackedType_ = BeAttackedType::NORMAL_HITED;
					isReturnClutch_ = true;
					isStretching_ = false;
					isPullBackObj_ = false;
					wireTip_->SetNeglect(false);
				}

			} else if (other.GetSubTag() == "slap_attack") {
				behaviorRequest_ = PlayerState::BeAttacked;
				beAttackedType_ = BeAttackedType::SLAP_ATTACKED;
				isReturnClutch_ = true;
				isStretching_ = false;
				isPullBackObj_ = false;
				wireTip_->SetNeglect(false);
			} else if (other.GetSubTag() == "swing_hand") {
				behaviorRequest_ = PlayerState::BeAttacked;
				beAttackedType_ = BeAttackedType::SLAP_ATTACKED;
				isReturnClutch_ = true;
				isStretching_ = false;
				isPullBackObj_ = false;
				wireTip_->SetNeglect(false);
			}
		}
	}
}

void Player::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "boss_core") {
	}
}


void Player::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////


void Player::AdaptAdjustment() {
	transform_->SetTranslaion(adjustItem_->GetValue<Vector3>(groupName_, "pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　自動で移動させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::AutoMove(const Vector3& velocity) {
	velocity_ = velocity;
	Vector3 pos = transform_->GetTranslation();
	pos += velocity_ * GameTimer::DeltaTime();
	transform_->SetTranslaion(pos);
	playerAnimator_->NowToAfterTransition("move");
	isAutoMove_ = true;
	targetRotate = rightRotate;
	nowRotate = LerpShortAngle(nowRotate, targetRotate, 0.1f);

	transform_->SetQuaternion(Quaternion::AngleAxis(90.0f * toRadian, Vector3::UP()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Setter系
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::SetInverMatrix(const Matrix4x4& inver) {
	inverMat_ = inver;
}

void Player::SetCameraZDis(float z) {
	camerazDis_ = z;
}


void Player::SetFalsePullBack() {
	isPullBackObj_ = false;
	isStretching_ = false;
	isNearBack_ = false;
	isReturnClutch_ = true;
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
			if (isNearBack_) {
				ImGui::Text("True");
			}
			ImGui::TreePop();
		}

		ShowEasingDebug(easingIndex_);

		ImGui::DragFloat("throwSpeed", &throwSpeed_, 1.0f);

		BaseGameObject::Debug_Gui();

		playerAnimator_->Debug_Gui();

		ImGui::End();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Wire")) {
		ImGui::DragFloat2("Wire", &clutchEnd_.x);
		ImGui::TreePop();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}
#endif
