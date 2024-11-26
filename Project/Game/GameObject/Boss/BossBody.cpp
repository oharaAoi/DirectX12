#include "BossBody.h"

BossBody::BossBody() {}
BossBody::~BossBody() {}

void BossBody::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::Init() {
	BaseGameObject::Init();
	SetObject("boss_Body.gltf");
	SetAnimater("./Game/Resources/Model/Boss_Body/", "boss_Body.gltf", true, false, true);

	// colliderの設定
	SetMeshCollider("boss_body");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", defalutPos_);
	adjust->AddItem(groupName_, "startPos", startPos_);

	// 調整項目の適応
	AdaptAdjustment();

	transform_->SetTranslaion(startPos_);

	animationTime_ = 0.0f;
	animationTransitionTime_ = 1.0f;

	nowAnimatonName_ = "Stand_by";
	waitAnimationName_ = "Stand_by";

	animetor_->SetTransitionAnimation(nowAnimatonName_, "Stand_by");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::Update() {

	animationTime_ += GameTimer::DeltaTime();
	if (nowAnimatonName_ == waitAnimationName_) {
		animationTime_ = std::fmod(animationTime_, animetor_->GetAnimationDuration());
	} else if (animationTime_ >= animetor_->GetAnimationDuration()) {
		if (!animetor_->GetIsAnimationChange()) {
			ChangeAnimation(nowAnimatonName_, waitAnimationName_);
		}
	}

	if (animetor_->GetIsAnimationChange()) {
		if (preAnimatonName_ == waitAnimationName_) {
			animationTime_ = std::fmod(animationTime_, animetor_->GetAnimationDuration());
		}
	}
	
	if (animetor_ != nullptr) {
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	defalutPos_ = adjust->GetValue<Vector3>(groupName_, "pos");
	startPos_ = adjust->GetValue<Vector3>(groupName_, "startPos");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定の追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "throwMissile") {
		isDecrementHp_ = true;

		ChangeAnimation(nowAnimatonName_, "Damage");
	}
}

void BossBody::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void BossBody::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Animationの変更
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBody::ChangeAnimation(const std::string& preAnime, const std::string& afterAnime) {
	if (nowAnimatonName_ == afterAnime) {
		return;
	}

	animetor_->SetTransitionAnimation(preAnime, afterAnime);
	preAnimatonName_ = preAnime;
	nowAnimatonName_ = afterAnime;
}

void BossBody::NowToAfterAnimation(const std::string& afterAnime) {
	if (nowAnimatonName_ == afterAnime) {
		return;
	}

	animetor_->SetTransitionAnimation(nowAnimatonName_, afterAnime);
	preAnimatonName_ = nowAnimatonName_;
	nowAnimatonName_ = afterAnime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossBody::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}

void BossBody::Debug_Gui() {
	ImGui::Begin("Boss_Body");
	transform_->Debug_Gui();
	ImGui::End();
}
#endif