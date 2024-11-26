#include "BossUI.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

BossUI::BossUI(Boss* pBoss) { pBoss_ = pBoss; }
BossUI::~BossUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Init() {
	sprites_["bossHp"] = Engine::CreateSprite("kari_bossHp.png");
	sprites_["bossHp"] = Engine::CreateSprite("kari_bossHp.png");

	sprites_["coreClutch"] = Engine::CreateSprite("star.png");
	sprites_["coreClutch"]->SetScale(Vector2{ 0.4f, 0.4f });

	planes_["coreClutch"] = std::make_unique<BaseGameObject>();
	planes_["coreClutch"]->Init();
	planes_["coreClutch"]->SetObject("plane.obj");
	planes_["coreClutch"]->SetTexture("star.png");
	planes_["coreClutch"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	planes_["coreClutch"]->GetTransform()->SetScale(Vector3(3.0f, 3.0f, 1.0f));

	planes_["canClutchUI"] = std::make_unique<BaseGameObject>();
	planes_["canClutchUI"]->Init();
	planes_["canClutchUI"]->SetObject("plane.obj");
	planes_["canClutchUI"]->SetTexture("point.png");
	planes_["canClutchUI"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	planes_["canClutchUI"]->GetTransform()->SetScale(Vector3(3.0f, 3.0f, 1.0f));


	// -------------------------------------------------
	// ↓ 調整項目系
	// -------------------------------------------------
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "hp_pos", sprites_["bossHp"]->GetTranslate());

	// 調整項目の適応
	AdaptAdjustment();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Update(float bossHp, const Matrix4x4& vpvpMat) {
	sprites_["bossHp"]->SetUvMaxSize(Vector2(std::clamp(bossHp / 100.0f, 0.0f, 1.0f), 1.0f));

	Vector3 pos = Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix() * vpvpMat);
	Vector2 coreScreenPos = { pos.x, pos.y };
	sprites_["coreClutch"]->SetTranslate(coreScreenPos);


	sprites_["bossHp"]->Update();
	sprites_["coreClutch"]->Update();

	planes_["coreClutch"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
	planes_["coreClutch"]->Update();

	if (uiTime_ > 0.0f) {
		uiTime_ += 9.0f * GameTimer::DeltaTime();
		float sinSize = std::lerp(minVal_, maxVal_, (sinf(uiTime_) + 1.0f) * 0.5f);
		planes_["canClutchUI"]->GetTransform()->SetScale({ sinSize,sinSize,1.0f });
		planes_["canClutchUI"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
		planes_["canClutchUI"]->Update();
	}
	else if (uiTime_ <= 0.0f) {
		uiTime_ += 3.0f * GameTimer::DeltaTime();
		float t = uiTime_ + 1.0f;
		float uiSize = std::lerp(20.0f, 2.5f, t);
		planes_["canClutchUI"]->GetTransform()->SetScale({ uiSize,uiSize,1.0f });
		planes_["canClutchUI"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
		planes_["canClutchUI"]->Update();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Draw() {
	sprites_["bossHp"]->Draw();
}

void BossUI::Draw3dObject(bool canAttackBoss,bool isPullSign){
	if (canAttackBoss) {
		planes_["coreClutch"]->Draw();
	}
	if (isPullSign) {
		planes_["canClutchUI"]->Draw();
	}
	else {
		uiTime_ = -1.0f;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	sprites_["bossHp"]->SetTranslate(adjust->GetValue<Vector2>(groupName_, "hp_pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug調整
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossUI::Debug_Gui() {
	if (ImGui::TreeNode("BossUI")) {
		sprites_["bossHp"]->Debug_Gui("bossHp");
		ImGui::TreePop();
	}
}
#endif