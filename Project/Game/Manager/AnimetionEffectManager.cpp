#include "AnimetionEffectManager.h"

AnimetionEffectManager::~AnimetionEffectManager() {}

AnimetionEffectManager* AnimetionEffectManager::GetInstance() {
	static AnimetionEffectManager instance;
	return &instance;
}

void AnimetionEffectManager::Finalize() {
	effectList_.clear();
}


void AnimetionEffectManager::Init() {
}

void AnimetionEffectManager::Update() {
	for (std::list<EffectData>::iterator it = effectList_.begin(); it != effectList_.end();) {
		if ((*it).pTranslation != nullptr) {
			(*it).effect->GetTransform()->SetTranslaion(*(*it).pTranslation);
		}

		if ((*it).pQuaternion != nullptr) {
			(*it).effect->GetTransform()->SetQuaternion(*(*it).pQuaternion);
		}

		(*it).effect->Update();

		if ((*it).effect->GetAnimetor()->GetAnimationClip()->GetIsAnimationFinish()) {
			it = effectList_.erase(it);
			continue;
		}

		++it;
	}
}

void AnimetionEffectManager::Draw() const {
	for (std::list<EffectData>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it).effect->Draw();
		++it;
	}
}

void AnimetionEffectManager::AddListEffect(const std::string& directoryPath, const std::string& filePath, const WorldTransform* parentWorldTransform,
										   bool isLighting, const Vector3& scale, const Quaternion& rotate, const Vector3& translation) {

	auto& newObj = GetInstance()->effectList_.emplace_back();
	newObj.effect = std::make_unique<BaseGameObject>();
	newObj.effect->Init();
	newObj.effect->SetObject(filePath);
	newObj.effect->SetAnimater(directoryPath, filePath, true, false, false);
	newObj.effect->SetIsLighting(isLighting);

	newObj.effect->GetTransform()->SetScale(scale);
	if (parentWorldTransform != nullptr) {
		newObj.pTranslation = &parentWorldTransform->GetTranslation();
		newObj.pQuaternion = &parentWorldTransform->GetQuaternion();
	} else {
		newObj.pTranslation = nullptr;
		newObj.pQuaternion = nullptr;
		newObj.effect->GetTransform()->SetTranslaion(translation);
		newObj.effect->GetTransform()->SetQuaternion(rotate);
	}
}

