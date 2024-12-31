#include "ObjectAxis.h"
#include "Engine/Assets/GameObject/BaseGameObject.h"

ObjectAxis::ObjectAxis() {
}

ObjectAxis::~ObjectAxis() {
}

void ObjectAxis::Finalize() {
}

void ObjectAxis::Init() {
	transform_ = Engine::CreateWorldTransform();

	model_ = nullptr;
	model_ = ModelManager::GetModel("axis.obj");
	materials.clear();
	for (const auto& material : model_->GetMaterialData()) {
		materials.emplace_back(Engine::CreateMaterial(material.second));
	}
}

void ObjectAxis::Update(const Matrix4x4& parentWorldMat) {
	transform_->Update(parentWorldMat);
}

void ObjectAxis::Draw() const {
	Render::DrawModel(model_, transform_.get(), materials);
}
