#include "BossAttackEditer.h"

BossAttackEditer::BossAttackEditer() {}
BossAttackEditer::~BossAttackEditer() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::Init() {
	controlPoint_.clear();
	controlPointObjects_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Update() {
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		controlPointObjects_[index]->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Draw() const {
	// -------------------------------------------------
	// ↓ 線の描画
	// -------------------------------------------------
	if (controlPoint_.size() > 1) {
		Engine::SetPipeline(PipelineType::PrimitivePipeline);
		for (size_t index = 0; index < controlPoint_.size() - 1; ++index) {
			Render::DrawLine(controlPoint_[index], controlPoint_[index + 1], Vector4(1, 1, 1, 1));
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
