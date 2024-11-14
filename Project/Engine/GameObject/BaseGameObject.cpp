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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
	animetor_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::Update() {
	if (animetor_ != nullptr) {
		animetor_->Update();

		if (!animetor_->GetIsSkinning()) {
			transform_->Update(animetor_->GetAnimationMat());
		} else {
			Engine::SetSkinning(animetor_->GetSkinning(), model_->GetMesh(0));
			transform_->Update();
		}

	} else {
		transform_->Update();
	}

#ifdef _DEBUG
	// Debug表示
	if (objectAxis_ != nullptr) {
		objectAxis_->Update(transform_->GetWorldMatrix());
	}
#endif

	if (meshCollider_ != nullptr) {
		meshCollider_->Update(transform_.get());
	}

	worldPos_ = Vector3(0.0f, 0.0f, 0.0f) * transform_->GetWorldMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::Draw() const {
	Render::DrawModel(model_, transform_.get(), materials);

#ifdef _DEBUG
	// Debug表示
	if (objectAxis_ != nullptr) {
		objectAxis_->Draw();
	}
#endif
}

void BaseGameObject::SetMeshCollider(const std::string& tag) {
	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));
	meshCollider_->SetTag(tag);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　modelを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = nullptr;
	model_ = ModelManager::GetModel(objName);
	materials.clear();
	for (uint32_t oi = 0; oi < model_->GetMaterialsSize(); ++oi) {
		std::string name = model_->GetMesh(oi)->GetUseMaterial();
		materials.push_back(Engine::CreateMaterial(model_->GetMaterialData(name)));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　アニメーションを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName, bool isSkinning) {
	animetor_.reset(new Animetor);
	animetor_->LoadAnimation(directoryPath, objName, model_, isSkinning);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　materialの色を変える
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetColor(const Vector4& color) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetColor(color);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　lightingの設定を変更する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetIsLighting(bool isLighting) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetIsLighting(isLighting);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　materialのtextureを変更する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetTexture(const std::string& path) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetUseTexture(path);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_->Debug_Gui();
	model_->Debug_Gui("Test");

	Debug_Axis();

	if (animetor_ != nullptr) {
		animetor_->Debug_Gui();
	}
}

void BaseGameObject::Debug_Axis() {
	// debug姿勢表示のonoff
	if (!isDebugAxis_) {
		ImGui::Checkbox("debugAxis", &isDebugAxis_);

		if (isDebugAxis_) {
			SetObjectAxis();
		}
	} else {
		ImGui::Checkbox("debugAxis", &isDebugAxis_);

		if (!isDebugAxis_) {
			SetObjectAxis(false);
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Objectの姿勢を可視化させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetObjectAxis(bool isAxis) {
	if (isAxis) {
		objectAxis_ = std::make_unique<ObjectAxis>();
		objectAxis_->Init();
		isDebugAxis_ = true;
	} else if (objectAxis_ != nullptr) {
		objectAxis_->Finalize();
		objectAxis_ = nullptr;
		isDebugAxis_ = false;
	}
}
#endif // _DEBUG