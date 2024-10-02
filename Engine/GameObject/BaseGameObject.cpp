#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
}

void BaseGameObject::Update() {
	if (isAnimation_) {
		animeter_.Update();
		animeter_.ApplyAnimation(skeleton_);
		skeleton_.Update();
		skinning_.Update(skeleton_);
	}
	transform_.Update();
}

void BaseGameObject::Draw() const {
	Render::DrawModel(model_, transform_);
	//skeleton_.Draw();
}

void BaseGameObject::DrawSKinning() const {
	Render::DrawAnimationModel(model_, skinning_, transform_);
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = ModelManager::GetModel(objName);
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName) {
	animeter_.LoadAnimation(directoryPath, objName);
	skeleton_.CreateSkeleton(model_->GetNode());
	skeleton_.Init();
	skinning_ = Engine::CreateSkinning(skeleton_, model_->GetMesh(0), model_->GetSkinClusterData());
	isAnimation_ = true;
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_.ImGuiDraw();
}
#endif // _DEBUG