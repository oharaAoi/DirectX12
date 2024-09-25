#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	model_ = Engine::CreateModel("AnimatedCube.gltf");
	transform_ = Engine::CreateWorldTransform();
	animeter_.LoadAnimation("AnimatedCube.gltf");
}

void BaseGameObject::Update() {
	animeter_.Update(model_->GetRootNodeName());
	transform_.Update(animeter_.GetMatrix());
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