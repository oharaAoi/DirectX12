#pragma once
#include <memory>
#include "Engine/Assets/GameObject/BaseGameObject.h"
#include "Engine/Assets/Collider/MeshCollider.h"
#include "Engine/Lib/IJsonConverter.h"

class TestObject : public BaseGameObject {
public:

	struct Test : public IJsonConverter {
		Vector3 pos;
		float lifeTime;
		float speed;
		float hp;
		float test;

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
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

	Matrix4x4 swordMat_;
	std::unique_ptr<BaseGameObject> sword_;

	bool isCollision_;

	bool isLerp_ = false;

	Test test_;
};

