#include "BossLeftHand.h"

BossLeftHand::BossLeftHand() {}
BossLeftHand::~BossLeftHand() {}

void BossLeftHand::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Init() {
	BaseGameObject::Init();
	SetObject("Left_Hand.obj");

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	// 調整項目の適応
	AdaptAdjustment();

	moveIndex_ = 0;

	std::filesystem::path dire(attackDirectoryPath);
	if (!std::filesystem::exists(attackDirectoryPath)) {
		std::filesystem::create_directories(attackDirectoryPath);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Update() {
	if (isClicked_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	} else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (isAttackMove_) {
		AttackMove();
	}

	BaseGameObject::Update();

	meshCollider_->Update(transform_.get(), Vector3::ZERO());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw();

#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::LoadAllFile() {
	pAttackEditer_->LoadAllFile(attackDirectoryPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::AttackMove() {
	moveTime_ += GameTimer::DeltaTime();
	float t = moveTime_ / 1.0f;
	if (pAttackEditer_->GetHandTransforms().size() - 1 > moveIndex_) {
		/*Vector3 scale = transform_->GetScale();
		Quaternion rotate = transform_->GetQuaternion();
		Vector3 translate = transform_->GetTranslation();*/

		Vector3 scale = Vector3::Lerp(pAttackEditer_->GetHandTransforms()[moveIndex_].scale, pAttackEditer_->GetHandTransforms()[moveIndex_ + 1].scale, t);
		Vector3 translate = Vector3::Lerp(pAttackEditer_->GetHandTransforms()[moveIndex_].translate, pAttackEditer_->GetHandTransforms()[moveIndex_ + 1].translate, t);

		transform_->SetScale(scale);
		transform_->SetTranslaion(translate);
	}

	if (moveTime_ > 1.0f) {
		if (pAttackEditer_->GetHandTransforms().size() - 1 == moveIndex_) {
			isAttackMove_ = false;
		} else {
			moveIndex_++;
			moveTime_ = 0;
		}
	}
}

void BossLeftHand::CheckMouseCursorCollision(const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	// objectのscreen座標を求める
	Vector3 objectScreen = Transform(Vector3::ZERO(), transform_->GetWorldMatrix() * vpvpMat);
	objectScreenPos_ = Vector2(objectScreen.x, objectScreen.y);

	bool isNear = false;
	// 長さを取って距離が近かったら
	if ((mousePos - objectScreenPos_).Length() < 100.0f) {
		isNear = true;
	}

	// カーソルと手が近い時はtrue
	if (isNear) {
		if (!isClicked_) {
			if (Input::IsPressMouse(0)) {
				isClicked_ = true;
			}
		} else {
			if (Input::IsPressMouse(0)) {
				isClicked_ = false;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossLeftHand::Debug_Gui() {
	ImGui::Begin("Boss_LeftHand");
	transform_->Debug_Gui();

	ImGui::DragFloat2("objectScreenPos", &objectScreenPos_.x, 1.0f);

	if (isClicked_) {
		if (Input::IsTriggerKey(DIK_SPACE)) {
			pAttackEditer_->AddPoint(transform_->GetScale(), transform_->GetQuaternion(), transform_->GetTranslation());
		}
	}

	ImGui::Separator();
	ImGui::Text("Editer");
	pAttackEditer_->Debug_Gui(attackDirectoryPath);

	if (ImGui::Button("attackMove")) {
		isAttackMove_ = true;
		moveIndex_ = 0;
		moveTime_ = 0;
	}

	ImGui::End();
}
#endif