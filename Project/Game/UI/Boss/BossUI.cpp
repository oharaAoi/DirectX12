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
	sprites_["coreClutch"] = Engine::CreateSprite("star.png");
	sprites_["coreClutch"]->SetScale(Vector2{ 0.4f, 0.4f });

	obj3d_["coreClutch"] = std::make_unique<BaseGameObject>();
	obj3d_["coreClutch"]->Init();
	obj3d_["coreClutch"]->SetObject("plane.obj");
	obj3d_["coreClutch"]->SetTexture("star.png");
	obj3d_["coreClutch"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	obj3d_["coreClutch"]->GetTransform()->SetScale(Vector3(3.0f, 3.0f, 1.0f));

	obj3d_["canClutchUI"] = std::make_unique<BaseGameObject>();
	obj3d_["canClutchUI"]->Init();
	obj3d_["canClutchUI"]->SetObject("plane.obj");
	obj3d_["canClutchUI"]->SetTexture("point.png");
	obj3d_["canClutchUI"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	obj3d_["canClutchUI"]->GetTransform()->SetScale(Vector3(3.0f, 3.0f, 1.0f));

	// -------------------------------------------------
	// ↓ 調整項目系
	// -------------------------------------------------
	
	// 調整項目の適応
	
	// -------------------------------------------------
	// ↓ 変数の定義
	// -------------------------------------------------
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Update(float bossHp, const Matrix4x4& vpvpMat) {
	bossHp = bossHp;
	Vector3 pos = Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix() * vpvpMat);
	Vector2 coreScreenPos = { pos.x, pos.y };
	sprites_["coreClutch"]->SetTranslate(coreScreenPos);
	sprites_["coreClutch"]->Update();

	obj3d_["coreClutch"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
	obj3d_["coreClutch"]->Update();

	if (uiTime_ > 0.0f) {
		uiTime_ += 9.0f * GameTimer::DeltaTime();
		float sinSize = std::lerp(minVal_, maxVal_, (sinf(uiTime_) + 1.0f) * 0.5f);
		obj3d_["canClutchUI"]->GetTransform()->SetScale({ sinSize,sinSize,1.0f });
		obj3d_["canClutchUI"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
		obj3d_["canClutchUI"]->Update();
	}
	else if (uiTime_ <= 0.0f) {
		uiTime_ += 3.0f * GameTimer::DeltaTime();
		float t = uiTime_ + 1.0f;
		float uiSize = std::lerp(20.0f, 2.5f, t);
		obj3d_["canClutchUI"]->GetTransform()->SetScale({ uiSize,uiSize,1.0f });
		obj3d_["canClutchUI"]->GetTransform()->SetTranslaion(Transform(Vector3::ZERO(), pBoss_->GetBossCore()->GetTransform()->GetWorldMatrix()));
		obj3d_["canClutchUI"]->Update();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Draw() {

}

void BossUI::Draw3dObject(bool canAttackBoss,bool isPullSign){
	if (canAttackBoss) {
		obj3d_["coreClutch"]->Draw();
	}
	if (isPullSign) {
		obj3d_["canClutchUI"]->Draw();
	}
	else {
		uiTime_ = -1.0f;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::AdaptAdjustment() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug調整
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossUI::Debug_Gui() {
	if (ImGui::TreeNode("BossUI")) {
		ImGui::Begin("BossUI");
		sprites_["bossHp"]->Debug_Gui("bossHp");

		ImGui::TreePop();
		ImGui::End();
	}
}
#endif