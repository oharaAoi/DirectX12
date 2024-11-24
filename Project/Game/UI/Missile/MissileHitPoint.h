#pragma once
#include <memory>
#include "Engine/2d/Sprite.h"

class MissileHitPoint {
public:

	MissileHitPoint();
	~MissileHitPoint();

	void Init();
	void Update(const Matrix4x4& missileMat, const Matrix4x4& vpvpMat, float t);
	void Draw() const;

private:

	std::unique_ptr<Sprite> hitPoint_;
	std::unique_ptr<Sprite> hitPointGauge_;

	Vector2 hitPos_;

};

