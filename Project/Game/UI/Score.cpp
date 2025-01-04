#include "Score.h"
#include "Engine/Engine.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Utilities/AdjustmentItem.h"

Score::Score() {
}

Score::~Score() {
}

void Score::Init() {
	scoreParameter_.FromJson(AdjustmentItem::GetData("Score", scoreParameter_.tag));

	for (uint32_t oi = 0; oi < kMaxScoreDegit_; ++oi) {
		targetScore_[oi] = Engine::CreateSprite("numbers.png");
		targetScore_[oi]->SetTranslate(Vector2{ scoreParameter_.targetPos.x + scoreParameter_.interval * -float(oi), scoreParameter_.targetPos.y });
		targetScore_[oi]->SetDrawRange(textureSize_);
		targetScore_[oi]->ReSetTextureSize(textureSize_);

		uint32_t score = IntegerCount(kMaxScore_, oi + 1);
		targetScore_[oi]->SetLeftTop(NumberSpriteLt(score, textureSize_));
	}

	auto& score = scores_.emplace_back(Engine::CreateSprite("numbers.png"));
	score->SetTranslate(Vector2{ scoreParameter_.scorePos.x + scoreParameter_.interval * -1.0f, scoreParameter_.scorePos.y });
	score->SetDrawRange(textureSize_);
	score->ReSetTextureSize(textureSize_);
	score->SetLeftTop(NumberSpriteLt(0, textureSize_));

	slash_ = Engine::CreateSprite("slash.png");
	slash_->SetTranslate(scoreParameter_.slashPos);

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Score::Debug_Gui, this), "score");
#endif
}

void Score::Update(uint32_t score) {
	if (DegitCount(score) > scores_.size()) {
		auto& newScore = scores_.emplace_back(Engine::CreateSprite("numbers.png"));
		newScore->SetTranslate(Vector2{ scoreParameter_.scorePos.x + scoreParameter_.interval * float(scores_.size() + 1.0f)  , scoreParameter_.scorePos.y });
		newScore->SetDrawRange(textureSize_);
		newScore->ReSetTextureSize(textureSize_);
		newScore->SetLeftTop(NumberSpriteLt(0, textureSize_));
	}

	for (uint32_t oi = 0; oi < kMaxScoreDegit_; ++oi) {
		targetScore_[oi]->Update();
	}

	for (uint32_t oi = 0; oi < scores_.size(); ++oi) {
		uint32_t nowScore = IntegerCount(score, oi + 1);
		scores_[oi]->SetLeftTop(NumberSpriteLt(nowScore, textureSize_));
		scores_[oi]->Update();
	}

	slash_->Update();
}

void Score::Draw() const {
	for (uint32_t oi = 0; oi < kMaxScoreDegit_; ++oi) {
		targetScore_[oi]->Draw();
	}

	for (uint32_t oi = 0; oi < scores_.size(); ++oi) {
		scores_[oi]->Draw();
	}

	slash_->Draw();
}

#ifdef _DEBUG
void Score::Debug_Gui() {
	for (uint32_t oi = 0; oi < kMaxScoreDegit_; ++oi) {
		std::string id = "maxScore" + std::to_string(oi);
		if (ImGui::TreeNode(id.c_str())) {
			targetScore_[oi]->Debug_Gui();
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("parameter")) {
		ImGui::DragFloat2("targetPos", &scoreParameter_.targetPos.x, 1.0f);
		ImGui::DragFloat2("scorePos", &scoreParameter_.scorePos.x, 1.0f);
		ImGui::DragFloat2("slashPos", &scoreParameter_.slashPos.x, 1.0f);
		ImGui::DragFloat("interval", &scoreParameter_.interval, 1.0f);
		if (ImGui::Button("Save")) {
			AdjustmentItem::Save("Score", scoreParameter_.ToJson(scoreParameter_.tag));
		}

		for (uint32_t oi = 0; oi < kMaxScoreDegit_; ++oi) {
			targetScore_[oi]->SetTranslate(Vector2{ scoreParameter_.targetPos.x + scoreParameter_.interval * -float(oi), scoreParameter_.targetPos.y });
		}

		for (uint32_t oi = 0; oi < scores_.size(); ++oi) {
			scores_[oi]->SetTranslate(Vector2{ scoreParameter_.scorePos.x + scoreParameter_.interval * -float(oi), scoreParameter_.scorePos.y });
		}

		slash_->SetTranslate(scoreParameter_.slashPos);
	}
}
#endif