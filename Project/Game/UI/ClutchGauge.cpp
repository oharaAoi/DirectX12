#include "ClutchGauge.h"
#include "Engine/Engine.h"

ClutchGauge::ClutchGauge() {
}

ClutchGauge::~ClutchGauge() {
}

void ClutchGauge::Init() {
	core_[0] = Engine::CreateSprite("gauge.png");
	core_[1] = Engine::CreateSprite("gaugeBar.png");
		
	fall_[0] = Engine::CreateSprite("gauge.png");
	fall_[1] = Engine::CreateSprite("gauge.png");
}

void ClutchGauge::Update(float coreEne, float coreMax, float fallEne, float fallMax, bool isCore, bool isFall) {
	float coreRaito = coreEne / coreMax;
	float fallRaito = fallEne / fallMax;

	isCore_ = isCore;
	isFall_ = isFall;

	if (isCore) {
		core_[1]->SetUvMinSize({ 0.0f, 1.0f - coreRaito });
	}

	if (isFall) {
		core_[1]->SetUvMinSize({ 0.0f, 1.0f - fallRaito });
	}

	for (uint32_t oi = 0; oi < 2; ++oi) {
		core_[oi]->SetTranslate(pos_);
		core_[oi]->Update();
	}

	for (uint32_t oi = 0; oi < 2; ++oi) {
		fall_[oi]->SetTranslate(pos_);
		fall_[oi]->Update();
	}
}

void ClutchGauge::Draw() const {
	/*if (isCore_) {
		for (uint32_t oi = 0; oi < 2; ++oi) {
			core_[oi]->Draw();
		}
	}

	if (isFall_) {
		for (uint32_t oi = 0; oi < 2; ++oi) {
			fall_[oi]->Draw();
		}
	}*/
}

#ifdef _DEBUG
void ClutchGauge::Debug_Gui() {
	ImGui::Begin("ClutchGauge");
	ImGui::SliderFloat2("pos", &pos_.x, 0.0f, 1280.0f);
	ImGui::End();
}
#endif