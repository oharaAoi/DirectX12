#include "BaseGameObject.h"
#include "Engine/Assets/Collider/SphereCollider.h"
#include "Engine/Assets/Collider/BoxCollider.h"

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
	// アニメーションが設定されていない場合はTransformのみ更新
	if (animetor_ == nullptr) {
		transform_->Update();
		PostUpdate();
		return;
	}

	// アニメーションの制御をスクリプトで行っていない場合
	if (!animetor_->GetIsControlScript()) {
		animetor_->Update();

		if (animetor_->GetIsSkinning()) {
			Engine::SetSkinning(animetor_->GetSkinning(), model_->GetMesh(0));
		} else {
			transform_->Update(animetor_->GetAnimationMat());
			PostUpdate();
			return;
		}
	} else {
		if (animetor_->GetIsSkinning()) {
			Engine::SetSkinning(animetor_->GetSkinning(), model_->GetMesh(0));
		}
	}

	// デフォルトのTransform更新
	transform_->Update();
	PostUpdate();
}

void BaseGameObject::PostUpdate() {

#ifdef _DEBUG
	// Debug軸の更新
	if (objectAxis_ != nullptr) {
		objectAxis_->Update(transform_->GetWorldMatrix());
	}
#endif

	// Colliderの更新
	if (meshCollider_ != nullptr) {
		meshCollider_->Update(transform_.get());
	}

	if (collider_ != nullptr) {
		collider_->Update(QuaternionSRT{.scale = transform_->GetScale(),
						  .rotate = transform_->GetQuaternion(),
						  .translate = transform_->GetTranslation()}
		);
	}

	worldPos_ = Vector3(0.0f, 0.0f, 0.0f) * transform_->GetWorldMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::Draw() const {
	if (animetor_ == nullptr || !animetor_->GetIsSkinning()) {
		Render::DrawModel(model_, transform_.get(), materials);
	} else {
		Render::DrawModel(model_, transform_.get(), animetor_->GetSkinning()->GetVBV(), materials);
	}
}

void BaseGameObject::SetMeshCollider(const std::string& tag) {
	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));
	meshCollider_->SetTag(tag);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Colliderを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetCollider(uint32_t bitTag, ColliderShape shape) {
	if (shape == ColliderShape::SPHERE) {
		collider_ = std::make_unique<SphereCollider>();
	} else if (shape == ColliderShape::AABB || shape == ColliderShape::OBB) {
		collider_ = std::make_unique<BoxCollider>();
	}

	collider_->Init(bitTag, shape);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　modelを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = nullptr;
	materials.clear();

	model_ = ModelManager::GetModel(objName);
	for (const auto& material : model_->GetMaterialData()) {
		materials.emplace_back(Engine::CreateMaterial(material.second));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　アニメーションを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName, bool isSkinning, bool isLoop, bool isControlScript) {
	animetor_.reset(new Animator);
	animetor_->LoadAnimation(directoryPath, objName, model_, isSkinning, isLoop, isControlScript);
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
void BaseGameObject::Debug_Draw() {
	if (collider_ != nullptr) {
		collider_->Draw();
	}
	// Debug表示
	if (objectAxis_ != nullptr) {
		objectAxis_->Draw();
	}
}

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