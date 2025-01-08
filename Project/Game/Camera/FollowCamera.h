#pragma once
#include "Engine/Lib/IJsonConverter.h"
#include "Game/Camera/BaseCamera.h"
#include "Game/System/LockOn/LockOn.h"

class FollowCamera 
	: public BaseCamera {
public:

	struct Information : public IJsonConverter {
		Vector3 offset;

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("offset", offset)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "offset", offset);
		}
	};

public:

	FollowCamera();
	~FollowCamera();

	void Finalize() override;
	void Init() override;
	void Update() override;

	void RotateCamera();

	void Shake();

	void SetIsShake();

private:

	Vector3 CalcucOffset();

public:

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	const Matrix4x4& GetRotateMat() const { return rotateMat_; }

	void SetTarget(WorldTransform* target) { target_ = target; }

private:

	// ------------------- pointer ------------------- //
	LockOn* lockOn_ = nullptr;

	WorldTransform* target_;

	// ------------------- parameter ------------------- //
	
	float lockOnDestinationAngleY_;
	float destinationAngleY_;
	float destinationAngleX_;

	Information information_;

	Matrix4x4 rotateMat_;

	Vector3 eulerRotate_;

	// ------------------- 演出 ------------------- //

	bool isShake_ = false;

	float shakeTime_ = 0.0f;
	float shakeTimeLimit_ = 0.5f;

	float shakeStrength_ = 0.0f;

	Vector3 shakeVelocity_;

};

