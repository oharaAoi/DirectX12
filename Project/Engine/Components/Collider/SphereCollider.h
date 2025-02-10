#pragma once
#include "Engine/Components/Collider/ICollider.h"

class SphereCollider
	: public ICollider {
public:

	SphereCollider();
	~SphereCollider();

	void Init(uint32_t bitTag, ColliderShape shape) override;
	void Update(const QuaternionSRT& srt) override;
	void Draw() const override;

public:

	// ------------ 半径 ------------ // 
	void SetRadius(const float& radius) { std::get<Sphere>(shape_).radius = radius; }
	float GetRadius() const { return std::get<Sphere>(shape_).radius; }

private:



};

