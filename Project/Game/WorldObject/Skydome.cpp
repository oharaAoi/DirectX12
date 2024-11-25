#include "Skydome.h"

void Skydome::Finalize() {
}

void Skydome::Init() {
	BaseGameObject::Init();
	SetObject("skydome.obj");
}

void Skydome::Update() {
	BaseGameObject::Update();
}

void Skydome::Draw() const {
	BaseGameObject::Draw();
}
