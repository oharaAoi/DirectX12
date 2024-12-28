#pragma once
#include <memory>
#include "Engine/Collider/ICollider.h"

class AttackCollider
	: public ICollider {
public:

	AttackCollider();
	~AttackCollider();

	void Init(const std::string& tag, ColliderShape shape) override;
	void Update(const SRT& srt) override;
	void Draw() const override;

public:

	// ------------ 半径 ------------ // 
	void SetRadius(const float& radius) { std::get<Sphere>(shape_).radius = radius; }
	float GetRadius() const { return std::get<Sphere>(shape_).radius; }

private:

};

