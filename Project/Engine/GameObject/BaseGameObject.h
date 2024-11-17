#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/GameObject/ObjectAxis.h"
#include "Engine/Assets/Animetor.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Collider/Collider.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Assets/AnimetionClip.h"
#include "Engine/Collider/MeshCollider.h"

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

	Animetor* GetAnimetor() { return animetor_.get(); }

	const bool GetIsAnimationFinish() const { return animetor_->GetIsAnimationFinish(); }

	// -------------------------------------------------
	// ↓ Collider関連
	// -------------------------------------------------

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	void SetMeshCollider(const std::string& tag);

protected:

	Model* model_;
	std::vector<std::unique_ptr<Material>> materials;	// 後で変えたい

	std::unique_ptr<WorldTransform> transform_;
	std::unique_ptr<Animetor> animetor_ = nullptr;

	std::unique_ptr<ObjectAxis> objectAxis_; // objectの回転を可視化したもの

	std::unique_ptr<MeshCollider> meshCollider_; // 当たり判定を行うクラス

	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	Vector3 worldPos_;

	bool isAnimation_ = false;

#ifdef _DEBUG
	bool isDebugAxis_;
#endif // _DEBUG
};