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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossUI::Draw() {
	sprites_["bossHp"]->Draw();
}

void BossUI::Draw3dObject(bool canAttackBoss){
	if (canAttackBoss) {
		planes_["coreClutch"]->Draw();
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