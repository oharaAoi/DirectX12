#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
}

void BaseGameObject::Update() {
}

void BaseGameObject::Draw() const {
	Render::DrawModel(model_.get(), *transform_);
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = Engine::CreateModel(objName);
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
}
#endif // _DEBUG