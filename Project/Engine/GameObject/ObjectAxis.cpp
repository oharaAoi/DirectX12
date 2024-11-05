#include "ObjectAxis.h"
#include "Engine/GameObject/BaseGameObject.h"

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
	for (uint32_t oi = 0; oi < model_->GetMaterialsSize(); ++oi) {
		std::string name = model_->GetMesh(oi)->GetUseMaterial();
		materials.push_back(Engine::CreateMaterial(model_->GetMaterialData(name)));
	}
}

void ObjectAxis::Update(const Matrix4x4& parentWorldMat) {
	transform_->Update(parentWorldMat);
}

void ObjectAxis::Draw() const {
	Render::DrawModel(model_, transform_.get(), materials);
}
