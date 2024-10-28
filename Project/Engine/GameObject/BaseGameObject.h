#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/Animetor.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Collider/Collider.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Assets/AnimetionClip.h"

class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Finalize();
	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	void UpdateMatrix();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetObject(const std::string& objName);

	void SetAnimater(const std::string& directoryPath, const std::string& objName, bool isSkinning);

	WorldTransform* GetTransform() { return transform_.get(); }

	const Vector4& GetColor()const{ return color_; }
	void SetColor(const Vector4& color);

	void SetIsLighting(bool isLighting);

	bool IsSetAnimetor();

	const bool GetIsAnimationFinish() const { return animetor_->GetIsAnimationFinish(); }

	void SetTexture(const std::string& path);

	void SetAnimationControlScrilpt(const bool& isControl) { isAnimationControlScript_ = isControl; }

	Animetor* GetAnimetor() { return animetor_.get(); }

protected:

	Model* model_;
	std::vector<std::unique_ptr<Material>> materials;	// 後で変えたい

	std::unique_ptr<WorldTransform> transform_;
	std::unique_ptr<Animetor> animetor_ = nullptr;
	std::unique_ptr<AnimetionClip> animationClip_;

	bool isAnimation_ = false;
	bool isAnimationControlScript_;

	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	Vector3 worldPos_;
};