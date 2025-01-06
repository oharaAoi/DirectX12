#include "Ground.h"
#include "Engine/Editer/Window/EditerWindows.h"

Ground::Ground() {
}

Ground::~Ground() {
}

void Ground::Finalize() {
}

void Ground::Init() {
	BaseGameObject::Init();
	SetObject("ground2.obj");

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Ground::Debug_Gui, this), "ground");
#endif
}

void Ground::Update() {
	BaseGameObject::Update();
}

void Ground::Draw() const {
	BaseGameObject::Draw();
}

#ifdef _DEBUG
void Ground::Debug_Gui() {
	transform_->Debug_Gui();
}
#endif