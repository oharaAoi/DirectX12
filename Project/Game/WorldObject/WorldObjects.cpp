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

	objectMap_["Wheel"] = std::make_unique<BaseGameObject>();
	objectMap_["Wheel"]->Init();
	objectMap_["Wheel"]->SetObject("wheel.obj");
	objectMap_["Wheel"]->GetTransform()->SetTranslaion(Vector3(-22.4f, -1.3f, 345.0f));

	objectMap_["Wheel2"] = std::make_unique<BaseGameObject>();
	objectMap_["Wheel2"]->Init();
	objectMap_["Wheel2"]->SetObject("wheel.obj");
	objectMap_["Wheel2"]->GetTransform()->SetTranslaion(Vector3(140.4f, -1.3f, 65.0f));

	objectMap_["Wheel3"] = std::make_unique<BaseGameObject>();
	objectMap_["Wheel3"]->Init();
	objectMap_["Wheel3"]->SetObject("wheel.obj");
	objectMap_["Wheel3"]->GetTransform()->SetTranslaion(Vector3(-228.4f, -3.0f, 100.0f));
	objectMap_["Wheel3"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(90.0f * toRadian, Vector3::UP()));

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
