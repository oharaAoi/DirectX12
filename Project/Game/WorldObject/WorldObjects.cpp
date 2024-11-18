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

	objectMap_["Gate"] = std::make_unique<BaseGameObject>();
	objectMap_["Gate"]->Init();
	objectMap_["Gate"]->SetObject("gate.obj");
	objectMap_["Gate"]->GetTransform()->SetTranslaion(Vector3(3.4f, -1.3f, 17.0f));

}

void WorldObjects::Update() {
	for (auto& obj : objectMap_) {
		obj.second->Update();
	}

	objectMap_["Gate"]->Debug_Gui();
}

void WorldObjects::Draw() const {
	for (auto& obj : objectMap_) {
		obj.second->Draw();
	}
}
