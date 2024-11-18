#include "GameEffect.h"

std::vector<Vector3> GameEffect::framePopPos_;

GameEffect::GameEffect() {
}

GameEffect::~GameEffect() {
}

void GameEffect::Init() {
	framePopPos_.clear();
	downEffets_.clear();
}

void GameEffect::Update() {
	for (uint32_t oi = 0; oi < framePopPos_.size(); ++oi) {
		Pop(4.0f, framePopPos_[oi]);
	}
	framePopPos_.clear();

	for (auto& effect : downEffets_) {
		effect->Update();
	}

	downEffets_.remove_if([](auto& effect) {return !effect->GetIsAlive(); });
}

void GameEffect::Draw() const {
	for (auto& effect : downEffets_) {
		effect->Draw();
	}
}

void GameEffect::Pop(int popNum, const Vector3& popPos) {
	for (uint32_t oi = 0; oi < popNum; ++oi) {
		auto& newEffect = downEffets_.emplace_back(std::make_unique<DownEffect>());
		newEffect->Init();
		Vector3 randomeVeclocity = Vector3(RandomFloat(-1, 1), RandomFloat(-1, 1), RandomFloat(-1, 1)).Normalize();
		newEffect->SetMove(popPos, randomeVeclocity, 1.0f);
	}
}

void GameEffect::NewPop(const Vector3& popPos) {
	framePopPos_.push_back(popPos);
}
