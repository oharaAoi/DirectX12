#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	model_ = Engine::CreateModel("skin.obj");
	transform_ = Engine::CreateWorldTransform();
}

void BaseGameObject::Update() {
	transform_.Update();
}

void BaseGameObject::Draw() const {
	Render::DrawModel(model_.get(), transform_);
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = Engine::CreateModel(objName);
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_.ImGuiDraw();
}
#endif // _DEBUG