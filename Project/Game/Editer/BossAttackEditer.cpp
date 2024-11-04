#include "BossAttackEditer.h"

BossAttackEditer::BossAttackEditer() {}
BossAttackEditer::~BossAttackEditer() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::Init() {
	controlPoint_.clear();
	controlPointObjects_.clear();

	segmentCount_ = 50;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Update() {
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		controlPointObjects_[index]->Update();

		if (index != selectIndex_) {
			controlPointObjects_[index]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	movePoints_.clear();
	if (controlPoint_.size() > 4) {
		for (uint32_t oi = 0; oi < segmentCount_; ++oi) {
			float t = (1.0f / static_cast<float>(segmentCount_)) * oi;
			Vector3 pos = CatmullRomPosition(controlPoint_, t);
			movePoints_.push_back(pos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Draw() const {
	// -------------------------------------------------
	// ↓ 線の描画
	// -------------------------------------------------
	if (movePoints_.size() > 4) {
		Engine::SetPipeline(PipelineType::PrimitivePipeline);
		for (size_t index = 0; index < movePoints_.size() - 1; ++index) {
			Render::DrawLine(movePoints_[index], movePoints_[index + 1], Vector4(1, 1, 1, 1));
		}
	}

	// -------------------------------------------------
	// ↓ pointのオブジェクトの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		controlPointObjects_[index]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ポイントの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::AddPoint() {
	ImGui::DragFloat3("newPoint", &newPoint_.x, 0.1f);
	if (ImGui::Button("AddPoint")) {
		controlPoint_.push_back(newPoint_);
		auto& newObject = controlPointObjects_.emplace_back(std::make_unique<BaseGameObject>());
		newObject->Init();
		newObject->SetObject("cube.obj");
		newObject->GetTransform()->SetTranslaion(newPoint_);
		newObject->GetTransform()->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ポイントの削除
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::DeletePoint(const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	std::vector<Vector2> objectPos;
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		// objectのscreen座標を求める
		Vector3 objectScreen = Transform(Vector3::ZERO(), controlPointObjects_[index]->GetTransform()->GetWorldMatrix() * vpvpMat);
		Vector2 objectScreenPos = Vector2(objectScreen.x, objectScreen.y);
		objectPos.push_back(objectScreenPos);
	}

	for(uint32_t index = 0; index < controlPointObjects_.size(); ++index)
	// 長さを取って距離が近かったら
	if ((mousePos - objectPos[index]).Length() < 20.0f) {
		if (Input::IsTriggerMouse(0)) {
			selectIndex_ = index;
			controlPointObjects_[index]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	// 選択されたポイントを削除する
	ImGui::Text("selectIndex : %d", selectIndex_);
	if (Input::IsTriggerKey(DIK_BACKSPACE)) {
		if (selectIndex_ >= 0 && selectIndex_ < controlPointObjects_.size()) {
			controlPointObjects_.erase(controlPointObjects_.begin() + selectIndex_);
		}

		if (selectIndex_ >= 0 && selectIndex_ < controlPoint_.size()) {
			controlPoint_.erase(controlPoint_.begin() + selectIndex_);
		}

		selectIndex_ = 999;
	}
}
