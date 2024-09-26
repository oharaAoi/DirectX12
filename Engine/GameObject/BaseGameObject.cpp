#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
}

void BaseGameObject::Update() {
	animeter_.Update(model_->GetRootNodeName());
	animeter_.ApplyAnimation(skeleton_);
	skeleton_.Update();
	transform_.Update();
}

void BaseGameObject::Draw() const {
	Render::DrawModel(model_.get(), transform_);
	skeleton_.Draw();
}

void BaseGameObject::SetObject(const std::string& directoryPath, const std::string& objName) {
	model_ = Engine::CreateModel(directoryPath, objName);
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName) {
	animeter_.LoadAnimation(directoryPath, objName);
	skeleton_.CreateSkeleton(model_->GetNode());
	skeleton_.Init();
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_.ImGuiDraw();
}
#endif // _DEBUG