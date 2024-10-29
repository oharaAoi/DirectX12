#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Playerの弾を撃つ方向
/// </summary>
class Reticle {
public:

	Reticle();
	~Reticle();

	void Init();
	void Update(const Vector3& playerPos, const Vector3& playerForward, const Matrix4x4& vpvpMat);
	void Draw() const;

	void Move();

	const Vector2 GetReticle2DPos() const { return reticleScreenPos_; }

private:

	std::unique_ptr<Sprite> reticle_;
	std::unique_ptr<BaseGameObject> reticle3D_;

	Vector2 offsetPos_;
	Vector2 reticleScreenPos_;
	Vector3 reticleWorldPos_;
	float lenght_;
};

