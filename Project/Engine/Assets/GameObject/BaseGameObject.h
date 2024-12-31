#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine/Engine.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/System/Manager/ModelManager.h"
#include "Engine/Assets/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/GameObject/ObjectAxis.h"
#include "Engine/Assets/Animation/Animator.h"

#include "Engine/Assets/Collider/MeshCollider.h"
#include "Engine/Assets/Collider/ICollider.h"
#include "Engine/Assets/Collider/SphereCollider.h"
#include "Engine/Assets/Collider/BoxCollider.h"

class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Finalize();
	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	void PostUpdate();

#ifdef _DEBUG
	void Debug_Draw();

	void Debug_Gui();

	void Debug_Axis();

	void SetObjectAxis(bool isAxis = true);
#endif // _DEBUG

	void SetObject(const std::string& objName);

	Model* GetModel() { return model_; }

	WorldTransform* GetTransform() { return transform_.get(); }

	// -------------------------------------------------
	// ↓ Material関連
	// -------------------------------------------------

	const Vector4& GetColor()const { return color_; }
	void SetColor(const Vector4& color);

	void SetIsLighting(bool isLighting);

	void SetTexture(const std::string& path);

	// -------------------------------------------------
	// ↓ Animation関連
	// -------------------------------------------------

	void SetAnimater(const std::string& directoryPath, const std::string& objName, bool isSkinning, bool isLoop, bool isControlScript);

	Animator* GetAnimetor() { return animetor_.get(); }

	// -------------------------------------------------
	// ↓ Collider関連
	// -------------------------------------------------

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	void SetMeshCollider(const std::string& tag);

	ICollider* GetCollider() { return collider_.get(); }
	void SetCollider(uint32_t bitTag, ColliderShape shape);

private:

	void SetColliderRadius(float radius) {
		if (auto sphere = dynamic_cast<SphereCollider*>(collider_.get())) {
			sphere->SetRadius(radius);
		}
	}

	void SetColliderSize(const Vector3& size) {
		if (auto box = dynamic_cast<BoxCollider*>(collider_.get())) {
			box->SetSize(size);
		}
	}

protected:

	Model* model_ = nullptr;
	std::vector<std::unique_ptr<Material>> materials;	// 後で変えたい

	std::unique_ptr<WorldTransform> transform_ = nullptr;
	std::unique_ptr<Animator> animetor_ = nullptr;

	std::unique_ptr<MeshCollider> meshCollider_ = nullptr; // 当たり判定を行うクラス
	std::unique_ptr<ICollider> collider_ = nullptr;

	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	Vector3 worldPos_ = { 1.0f, 1.0f, 1.0f};

	bool isAnimation_ = false;

#ifdef _DEBUG
	bool isDebugAxis_;

	std::unique_ptr<ObjectAxis> objectAxis_ = nullptr; // objectの回転を可視化したもの
#endif // _DEBUG
};