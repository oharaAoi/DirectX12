#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	
}

PlayerBullet::~PlayerBullet() {
}

void PlayerBullet::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Init() {
	isAlive_ = true;
	BaseGameObject::Init();
	SetObject("razer.obj");
	speed_ = 2.0f;

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));
	meshCollider_->SetTag("playerBullet");

	//SetObjectAxis();

	meshCollider_->SetOnCollisionCallBack([this](MeshCollider& other) {
		this->OnCollision(other);
	 });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Update() {
	Vector3 worldOffset = Transform(offset_, cameraMat_);
	transform_->SetTranslaion( worldOffset);

	BaseGameObject::Update();

	meshCollider_->Update(transform_.get(), Vector3::ZERO());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw(); 

	/*Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerBullet::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "star") {
		isAlive_ = false;
		Player::AddScore(40);
		KnockDownEnemy::SetObj("star.obj", 40);
		AudioPlayer::SinglShotPlay("hit.mp3", 0.3f);
		GameEffect::NewPop(other.GetObbCenter());

	} else if(other.GetTag() == "balloon") {
		isAlive_ = false;
		Player::AddScore(20);
		KnockDownEnemy::SetObj("balloon.obj", 20);
		AudioPlayer::SinglShotPlay("hit.mp3", 0.3f);
		GameEffect::NewPop(other.GetObbCenter());

	} else if (other.GetTag() == "jet") {
		isAlive_ = false;
		Player::AddScore(60);
		KnockDownEnemy::SetObj("jet.obj", 60);
		AudioPlayer::SinglShotPlay("hit.mp3", 0.3f);
		GameEffect::NewPop(other.GetObbCenter());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　World上の座標を返す
//////////////////////////////////////////////////////////////////////////////////////////////////

const Vector3 PlayerBullet::GetWorldPos() const {
	return Transform(Vector3::ZERO(), transform_->GetWorldMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成時の座標を指定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::SetPopPos(const Vector3& pos) {
	transform_->SetTranslaion(pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void PlayerBullet::Debug_Gui(const std::string& name) {
	ImGui::Begin(name.c_str());
	transform_->Debug_Quaternion();
	ImGui::End();
}
#endif