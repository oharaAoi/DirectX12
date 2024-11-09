#include "BossRightHand.h"

BossRightHand::BossRightHand() {
}

BossRightHand::~BossRightHand() {
}

void BossRightHand::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::Init() {
	BaseGameObject::Init();
	SetObject("Right_Hand.obj");

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

void BossRightHand::Update() {
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

void BossRightHand::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw();

#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

void BossRightHand::LoadAllFile() {
	pAttackEditer_->LoadAllFile(attackDirectoryPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　手の移動を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::AttackMove() {
	moveTime_ += GameTimer::DeltaTime();
	if (pAttackEditer_->GetHandMoves().size() - 1 > moveIndex_) {
		/*Vector3 scale = transform_->GetScale();
		Quaternion rotate = transform_->GetQuaternion();
		Vector3 translate = transform_->GetTranslation();*/

		float t = moveTime_ / pAttackEditer_->GetHandMoves()[moveIndex_].moveTimeLimit;
		int easeType = pAttackEditer_->GetHandMoves()[moveIndex_].easeType;

		Vector3 scale = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].scale, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].scale, CallEasingFunc(easeType, t));
		Vector3 translate = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].translate, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].translate, CallEasingFunc(easeType, t));

		transform_->SetScale(scale);
		transform_->SetTranslaion(translate);
	}

	if (moveTime_ > 1.0f) {
		if (pAttackEditer_->GetHandMoves().size() - 1 == moveIndex_) {
			isAttackMove_ = false;
		} else {
			moveIndex_++;
			moveTime_ = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　マウスカーソルとあっているかを確認
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::CheckMouseCursorCollision(const Matrix4x4& vpvpMat) {
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
void BossRightHand::Debug_Gui() {
	ImGui::Begin("Boss_RightHand");
	ImGui::BulletText("Set_Parameter");
	ImGui::Indent(20.0f);
	transform_->Debug_Gui();
	
	ShowEasingDebug(easeType_);	// easingを決める
	ImGui::DragFloat("moveTimeLimit", &moveTimeLimit_, 0.1f, 0.1f, 10.0f);	// 移動時間を決める

	// 決定をする
	if (isClicked_) {
		if (Input::IsTriggerKey(DIK_SPACE)) {
			pAttackEditer_->AddPoint(transform_->GetScale(), transform_->GetQuaternion(), transform_->GetTranslation(), easeType_, moveTimeLimit_);
		}
	}
	ImGui::Unindent(20.0f);

	ImGui::Separator();
	ImGui::BulletText("Editer");
	ImGui::Indent(20.0f);
	pAttackEditer_->Debug_Gui(attackDirectoryPath);

	if (pAttackEditer_->GetHandMoves().size() != 0) {
		if (ImGui::Button("attackMove")) {
			isAttackMove_ = true;
			moveIndex_ = 0;
			moveTime_ = 0;
		}
	}
	ImGui::Unindent(20.0f);

	ImGui::Separator();
	ImGui::BulletText("Debug_Parameter");
	ImGui::Indent(20.0f);
	Debug_Axis();
	ImGui::DragFloat2("objectScreenPos", &objectScreenPos_.x, 1.0f);
	ImGui::Unindent(20.0f);

	ImGui::End();
}
#endif