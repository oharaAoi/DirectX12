#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

class TestObject : public BaseGameObject {
public:

	TestObject();
	~TestObject();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);
	

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	bool isCollision_;

	float animationTime_;

	float transitionAnimationTime_;
	float transitionAnimationTimeLimit_;

	bool isLerp_ = false;

};

