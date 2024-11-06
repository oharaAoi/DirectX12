#include "KnockDownEnemy.h"

std::string KnockDownEnemy::knockObjName_;
bool KnockDownEnemy::isChange_;
uint32_t KnockDownEnemy::knockScore_;

KnockDownEnemy::KnockDownEnemy() {
}

KnockDownEnemy::~KnockDownEnemy() {
}

void KnockDownEnemy::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void KnockDownEnemy::Init() {
	BaseGameObject::Init();

	rotateCount_ = 0.0f;
	rotateTime_ = 2.0f;

	screenPos_ = { 1000.0f, 500.0f };
	originPos_ = { 1030.0f, 580.0f };

	numberSpriteSize_ = { 52.0f, 52.0f };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void KnockDownEnemy::Update() {
	// objectの更新があるか
	if (isChange_) {
		SetObject(knockObjName_);
		rotateCount_ = rotateTime_;
		rotateAngle_ = 0.0f;
		CalculateionScore();
	}

	// 回転する時間が0だったら映さない
	if (rotateCount_ == 0) {
		return;
	} else {
		rotateCount_ -= GameTimer::DeltaTime();
		if (rotateCount_ < 0) {
			rotateCount_ = 0.0f;
		}
	}

	// -------------------------------------------------
	// ↓ 回転の更新と位置の更新
	// -------------------------------------------------
	rotateAngle_+= 360.0f * GameTimer::DeltaTime();
	transform_->SetTranslaion(worldPos_);
	transform_->SetScale({ 0.5f, 0.5f, 0.5f });
	transform_->SetQuaternion(Quaternion::AngleAxis(rotateAngle_ * toRadian, {0.0f, 1.0f, 0.0f}));

	BaseGameObject::Update();

	for (uint32_t index = 0; index < knockScoreArray_.size(); ++index) {
		knockScoreArray_[index]->Update();
	}

	isChange_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void KnockDownEnemy::Draw() const {
	if (rotateCount_ == 0.0f) {
		return;
	}
	BaseGameObject::Draw();

	Engine::SetPipeline(PipelineType::SpritePipeline);
	for (uint32_t index = 0; index < knockScoreArray_.size(); ++index) {
		knockScoreArray_[index]->Draw();
	}
}

void KnockDownEnemy::CalculateionScore() {
	knockScoreArray_.clear();
	knockScoreArray_.resize(DegitCount(knockScore_));
	for (uint32_t index = 0; index < knockScoreArray_.size(); ++index) {
		knockScoreArray_[index] = Engine::CreateSprite("number.png");
		knockScoreArray_[index]->SetTextureSize(numberSpriteSize_);
		knockScoreArray_[index]->SetRectRange(numberSpriteSize_);
		knockScoreArray_[index]->SetLeftTop(NumberSpriteLt(IntegerCount(knockScore_, index + 1), numberSpriteSize_));
		Vector2 pos = originPos_;
		pos.x -= numberSpriteSize_.x * index;
		knockScoreArray_[index]->SetCenterPos(pos);
	}
}

#ifdef _DEBUG
void KnockDownEnemy::Debug_Gui() {
	if (ImGui::TreeNode("knockDownEnemy")) {
		ImGui::Begin("knockDownEnemy");
		ImGui::DragFloat2("originPos", &originPos_.x, 1.0f);
		ImGui::DragFloat2("screenPos", &screenPos_.x, 1.0f);
		ImGui::DragFloat3("worldPos", &worldPos_.x, 1.0f);
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif

void KnockDownEnemy::SetObj(const std::string& name, uint32_t score) {
	knockObjName_ = name;
	knockScore_ = score;
	isChange_ = true;
}
