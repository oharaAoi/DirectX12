#pragma once
#include "Engine/Collider/ICollider.h"
#include <string>

class BoxCollider 
	: public ICollider {
public:

	BoxCollider();
	~BoxCollider();

	void Init(const std::string& tag, ColliderShape shape) override;
	void Update() override;
	void Draw() const override;

public:

	
private:


};
