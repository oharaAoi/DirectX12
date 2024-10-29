#include "Reticle.h"

Reticle::Reticle() {}
Reticle::~Reticle() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Init() {
	reticle_ = Engine::CreateSprite("reticle.png");
	reticle3D_ = std::make_unique<BaseGameObject>();
	reticle3D_->Init();
	reticle3D_->SetObject("cube.obj");

	lenght_ = 30.0f;
	offsetPos_ = { 0.0f, 0.0f };
	reticleScreenPos_ = { 0.0f, 0.0f };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Update(const Vector3& playerPos, const Vector3& playerForward, const Matrix4x4& vpvpMat) {
	Move();

	// -------------------------------------------------
	// ↓ プレイヤーの進行方向ベクトルの位置を求める
	// -------------------------------------------------
	reticleWorldPos_ = playerPos + (playerForward * lenght_);
	reticle3D_->GetTransform()->SetTranslaion(reticleWorldPos_);
	reticle3D_->Update();

	// -------------------------------------------------
	// ↓ Screen上の座標を求める
	// -------------------------------------------------
	Vector3 screenPos = Transform({ 0,0,0 }, reticle3D_->GetTransform()->GetWorldMatrix() * vpvpMat);
	reticleScreenPos_ = Vector2{ screenPos.x, screenPos.y } + offsetPos_;
	reticle_->SetCenterPos(reticleScreenPos_);

	// -------------------------------------------------
	// ↓ 更新を行う
	// -------------------------------------------------
	reticle_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Draw() const {
	//reticle3D_->Draw();

	Engine::SetPipeline(PipelineType::SpritePipeline);
	reticle_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Move() {
	Vector2 joyStick = Input::GetRightJoyStick();
	joyStick.y *= -1;
	offsetPos_ += joyStick * 5.0f;

	if (Input::IsPressKey(DIK_LEFT)) {
		offsetPos_.x -= 5.0f;
	}

	if (Input::IsPressKey(DIK_RIGHT)) {
		offsetPos_.x += 5.0f;
	}

	if (Input::IsPressKey(DIK_UP)) {
		offsetPos_.y -= 5.0f;
	}

	if (Input::IsPressKey(DIK_DOWN)) {
		offsetPos_.y += 5.0f;
	}
}
