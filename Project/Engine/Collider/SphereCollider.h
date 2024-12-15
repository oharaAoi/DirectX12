#pragma once
#include "Engine/Collider/ICollider.h"

class SphereCollider
	: public ICollider {
public:

	SphereCollider();
	~SphereCollider();

	void Init(const std::string& tag, ColliderShape shape) override;
	void Update() override;
	void Draw() const override;

public:

	// ------------ 半径 ------------ // 
	void SetRadius(const float& radius) { std::get<Sphere>(shape_).radius = radius; }
	float GetRadius() const { return std::get<Sphere>(shape_).radius; }

private:


};

