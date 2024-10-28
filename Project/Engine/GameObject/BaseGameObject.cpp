#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
	if (transform_ != nullptr) {
		transform_->Finalize();
	}
	if (animetor_ != nullptr) {
		animetor_->Finalize();
	}
	materials.clear();
}

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
	animetor_ = nullptr;
	isAnimationControlScript_ = false;
}

void BaseGameObject::Update() {
	if (!isAnimationControlScript_) {
		if (animetor_ != nullptr) {
			animetor_->Update();
		}
	}

	if (animationClip_) {
		animationClip_->Update();
		transform_->Update(animationClip_->GetMatrix());
	} else {
		transform_->Update();
	}

	worldPos_ = Vector3(0.0f, 0.0f, 0.0f) * transform_->GetWorldMatrix();
}

void BaseGameObject::Draw() const {
	if (animetor_ == nullptr) {
		Render::DrawModel(model_, transform_.get(), materials);
	} else {
		//Engine::SetSkinning(animetor_->GetSkinning(), model_->GetMesh(0));
		//model_->GetMesh(0)->CopyVertexData(animetor_->GetSkinning()->GetVertexData());
		//Render::DrawModel(model_, transform_.get(), materials);
		Render::DrawAnimationModel(model_, animetor_->GetSkinning(), transform_.get(), materials);
	}
}

void BaseGameObject::UpdateMatrix(){
	if(animetor_ != nullptr) {
		animetor_->Update();
	}

	if(animationClip_) {
		animationClip_->Update();
		transform_->Update(animationClip_->GetMatrix());
	} else {
		transform_->Update();
	}

	worldPos_ = Vector3(0.0f, 0.0f, 0.0f) * transform_->GetWorldMatrix();
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = nullptr;
	model_ = ModelManager::GetModel(objName);
	materials.clear();
	for (uint32_t oi = 0; oi < model_->GetMaterialsSize(); ++oi) {
		std::string name = model_->GetMesh(oi)->GetUseMaterial();
		materials.push_back(Engine::CreateMaterial(model_->GetMaterialData(name)));
	}
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName, bool isSkinning) {
	if (isSkinning) {
		animetor_.reset(new Animetor);
		animetor_->LoadAnimation(directoryPath, objName, model_);
	} else {
		animationClip_.reset(new AnimetionClip);
		animationClip_->LoadAnimation(directoryPath, objName, model_->GetRootNodeName());
	}
}

void BaseGameObject::SetColor(const Vector4& color) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetColor(color);
	}
}

void BaseGameObject::SetIsLighting(bool isLighting){
	for(size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetIsLighting(isLighting);
	}
}

bool BaseGameObject::IsSetAnimetor() {
	if (animetor_ == nullptr) {
		return false;
	} else {
		return true;
	}
}

void BaseGameObject::SetTexture(const std::string& path) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetUseTexture(path);
	}
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_->Debug_Gui();
	model_->Debug_Gui("Test");
}
#endif // _DEBUG