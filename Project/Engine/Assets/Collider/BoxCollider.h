#pragma once
#include "Engine/Assets/Collider/ICollider.h"
#include <string>

class BoxCollider 
	: public ICollider {
public:

	BoxCollider();
	~BoxCollider();

	void Init(uint32_t bitTag, ColliderShape shape) override;
	void Update(const SRT& srt) override;
	void Draw() const override;

public:

	void SetSize(const Vector3& size) { size_ = size; }
	
private:

	Vector3 size_;

};
