#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyRandom.h"

class DownEffect : public BaseGameObject {
public:

	DownEffect();
	~DownEffect();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void SetMove(const Vector3& pos, const Vector3& velocity, float lifeTime);

public:

	const bool GetIsAlive() const { return isAlive_; }

private:

	Vector3 velocity_;
	float lifeTime_;
	bool isAlive_;

	float speed_ = 2.0f;

	Vector3 rotateAxis_;
};

