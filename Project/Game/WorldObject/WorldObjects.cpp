#include "WorldObjects.h"

WorldObjects::WorldObjects() {
}

WorldObjects::~WorldObjects() {
}

void WorldObjects::Init() {
	objectMap_["Ground"] = std::make_unique<BaseGameObject>();
	objectMap_["Ground"]->Init();
	objectMap_["Ground"]->SetObject("ground.obj");

	objectMap_["Board"] = std::make_unique<BaseGameObject>();
	objectMap_["Board"]->Init();
	objectMap_["Board"]->SetObject("board.obj");
	objectMap_["Board"]->GetTransform()->SetTranslationZ(50);

}

void WorldObjects::Update() {
	for (auto& obj : objectMap_) {
		obj.second->Update();
	}
}

void WorldObjects::Draw() const {
	for (auto& obj : objectMap_) {
		obj.second->Draw();
	}
}
