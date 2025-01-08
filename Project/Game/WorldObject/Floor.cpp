#include "Floor.h"

Floor::Floor() {}
Floor::~Floor() {}

void Floor::Finalize() {
}

void Floor::Init() {
	BaseGameObject::Init();
	SetObject("floor.obj");
	SetIsLighting(false);

	transform_->translate_.y = -0.01f;
}

void Floor::Update() {
	BaseGameObject::Update();
}

void Floor::Draw() const {
	BaseGameObject::Draw();
}
