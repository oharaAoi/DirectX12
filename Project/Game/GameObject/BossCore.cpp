#include "BossCore.h"

BossCore::BossCore() {}
BossCore::~BossCore() {}

void BossCore::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Init() {
	BaseGameObject::Init();
	SetObject("boss_core.obj");

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	// colliderの設定
	SetMeshCollider("boss_core");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	// 調整項目の適応
	AdaptAdjustment();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Update() {
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "player") {
		//AdaptAdjustment();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossCore::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
}
void BossCore::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}
void BossCore::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossCore::Debug_Gui() {
	ImGui::Begin("Boss_Core");
	transform_->Debug_Gui();
	ImGui::End();
}

void BossCore::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}
#endif