#include "TitlePlayer.h"
#include "Game/Scene/GameScene.h"


TitlePlayer::TitlePlayer() {}
TitlePlayer::~TitlePlayer() {}

void TitlePlayer::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::Init() {
	BaseGameObject::Init();
	SetObject("TitlePlayer.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, 0.0f, 0.0f });

	wire_ = std::make_unique<ClutchWire>();
	wire_->Init();
	wire_->GetTransform()->SetParentTranslation(transform_->GetTranslation());

	wireTip_ = std::make_unique<WireTip>();
	wireTip_->Init();

	// -------------------------------------------------
	// ↓ 調整項目の適応
	// -------------------------------------------------


	// -------------------------------------------------
	// ↓ 変数の初期化
	// -------------------------------------------------


}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::Update() {

	Vector3 pos = transform_->GetTranslation();
	if (isSnagged_) {
		clutchLerpTime_ += GameTimer::DeltaTime();
		pos = Lerp(pos, { clutchEnd_.x,clutchEnd_.y,pos.z }, CallEasingFunc(easingIndex_, powf(clutchLerpTime_, 2.0f)));
	}
	else {
		boundTime_ += 5.0f * GameTimer::DeltaTime();
		float sinSize = std::lerp(minBound_, maxBound_, (sinf(boundTime_) + 1.0f) * 0.5f);
		pos.y = 4.5f + sinSize;
	}
	transform_->SetTranslaion(pos);


	Vector2 wireTipPos = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Normalize() * wire_->GetTransform()->GetScale().y;
	wireTipPos += {transform_->GetTranslation().x, transform_->GetTranslation().y};
	wireTip_->GetTransform()->SetTranslaion({ wireTipPos.x,wireTipPos.y, transform_->GetTranslation().z });

	Stretching();


	BaseGameObject::Update();

	wire_->Update();
	wireTip_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::Draw() const {
	BaseGameObject::Draw();
	wire_->Draw();
	wireTip_->Draw();
}


void TitlePlayer::TitleSet() {
	clutchEnd_ = { 6.5f,20.0f };
	maxClutchLength_ = 50.0f;
	transform_->SetQuaternion(Quaternion::AngleAxis(3.1f, Vector3::UP()));
	transform_->SetTranslaion(Vector3{ 5.5f,4.5f,0.0f });
	isStretching_ = true;
	isStretchClutch_ = true;
	wire_->GetTransform()->SetTranslaion({ 0.8f,0.0f,0.0f });
	wireTip_->GetTransform()->SetTranslaion({ 5.0f,16.0f,0.0f });
}

void TitlePlayer::TitleEnd() {
	isStretching_ = false;
	isSnagged_ = true;
}


void TitlePlayer::ClearSet() {

	SetObject("Player.obj");
	maxClutchLength_ = 50.0f;
	transform_->SetQuaternion(Quaternion::AngleAxis(3.4f, Vector3::UP()));
	transform_->SetTranslaion(Vector3{ 3.0f,2.5f,-18.0f });

}

void TitlePlayer::ClearUpdate() {

	Vector2 wireTipPos = (clutchEnd_ - Vector2{ transform_->GetTranslation().x,transform_->GetTranslation().y }).Normalize() * wire_->GetTransform()->GetScale().y;
	wireTipPos += {transform_->GetTranslation().x, transform_->GetTranslation().y};
	wireTip_->GetTransform()->SetTranslaion({ wireTipPos.x,wireTipPos.y, transform_->GetTranslation().z });

	BaseGameObject::Update();

	wire_->Update();
	wireTip_->Update();

}

void TitlePlayer::Stretching() {

	if (isStretching_) {

		Vector3 nowScale = wire_->GetTransform()->GetScale();
		if (nowScale.y <= maxClutchLength_) {
			nowScale.y += stretchSpeed_ * GameTimer::DeltaTime();
			wire_->GetTransform()->SetScale(nowScale);
		}
		else {
			isStretching_ = false;
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	
}

void TitlePlayer::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	
}


void TitlePlayer::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////


void TitlePlayer::AdaptAdjustment() {
	transform_->SetTranslaion(adjustItem_->GetValue<Vector3>(groupName_, "pos"));
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Setter系
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::SetInverMatrix(const Matrix4x4& inver) {
	inverMat_ = inver;
}

void TitlePlayer::SetCameraZDis(float z) {
	camerazDis_ = z;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TitlePlayer::Debug_Gui() {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitlePlayer::Debug_Draw() {

}
#endif
