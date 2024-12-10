#include "Skydome.h"

Skydome::Skydome() {}
Skydome::~Skydome() {}

void Skydome::Finalize() {
}

void Skydome::Init() {
	BaseGameObject::Init();
	SetObject("skydome.obj");
	SetIsLighting(false);
}

void Skydome::Update() {
	BaseGameObject::Update();
}

void Skydome::Draw() const {
	BaseGameObject::Draw();
}
