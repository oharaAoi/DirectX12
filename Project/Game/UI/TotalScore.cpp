#include "TotalScore.h"

uint32_t TotalScore::playerScore_ = 0;

TotalScore::TotalScore() {
}

TotalScore::~TotalScore() {
}

void TotalScore::Init() {
	background_ = Engine::CreateSprite("scoreBackground.png");
	background_->SetCenterPos({ 1025.0f, 660.0f });

	numberSpriteSize_ = { 52.0f, 52.0f };
	originPos_ = { 1150.0f, 660.0f };
	
}

void TotalScore::Update(uint32_t playerScore) {
	playerScore_ = playerScore;
	// スコアの桁数を配列の数が違うかの確認
	if (DegitCount(playerScore_) != score_.size()) {
		AddArray();
	}

	for (size_t index = 0; index < score_.size(); ++index) {
		score_[index]->SetLeftTop(NumberSpriteLt(IntegerCount(playerScore_, int(index + 1)), numberSpriteSize_));
		score_[index]->Update();
	}

	background_->Update();
}

void TotalScore::Draw() const {
	background_->Draw();

	for (size_t index = 0; index < score_.size(); ++index) {
		score_[index]->Draw();
	}
}

void TotalScore::AddArray() {
	Vector2 pos = originPos_;
	pos.x -= (numberSpriteSize_.x - 10) * score_.size();
	// Spriteの作成をする
	auto& newSprite = score_.emplace_back(Engine::CreateSprite("number.png"));
	newSprite->SetCenterPos(pos);
	newSprite->SetTextureSize(numberSpriteSize_);
	newSprite->SetRectRange(numberSpriteSize_);
	// 新しく追加する数値を求める
	uint32_t number = IntegerCount(playerScore_, (int)score_.size());
	// spriteの左上座標を求める
	newSprite->SetLeftTop(NumberSpriteLt(number, numberSpriteSize_));

}

#ifdef _DEBUG
void TotalScore::Debug_Gui() {
	if (ImGui::TreeNode("TotalScore")) {
		ImGui::Begin("TotalScore");
		background_->Debug_Gui();
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif