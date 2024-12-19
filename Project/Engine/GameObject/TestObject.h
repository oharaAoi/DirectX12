#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Engine/Lib/IJsonConverter.h"

class TestObject : public BaseGameObject {
public:

	struct Test : public IJsonConverter {
		Vector3 pos;
		float lifeTime;
		float speed;
		float hp;

		json ToJson() const override {
			return JsonBuilder("testParame")
				.Add("pos", pos)
				.Add("lifeTime", lifeTime)
				.Add("speed", speed)
				.Add("hp", hp)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "pos", pos);
			fromJson(jsonData, "lifeTime", lifeTime);
			fromJson(jsonData, "speed", speed);
			fromJson(jsonData, "hp", hp);
		}
	};

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

	Test test_;
};

