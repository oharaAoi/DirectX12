#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
	animetor_ = nullptr;
}

void BaseGameObject::Update() {
	if (animetor_ != nullptr) {
		animetor_->Update();
	}
	transform_.Update();
}

void BaseGameObject::Draw() const {
	if (animetor_ == nullptr) {
		Render::DrawModel(model_, transform_);
	} else {
		Render::DrawAnimationModel(model_, animetor_->GetSkinning(), transform_);
	}
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = ModelManager::GetModel(objName);
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName) {
	animetor_ = std::make_unique<Animetor>();
	animetor_->LoadAnimation(directoryPath, objName, model_);
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_.ImGuiDraw();
}
#endif // _DEBUG