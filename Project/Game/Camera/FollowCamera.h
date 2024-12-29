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

private:

	Vector3 CalcucOffset();

public:

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	const Matrix4x4& GetRotateMat() const { return rotateMat_; }

private:

	// ------------------- pointer ------------------- //
	LockOn* lockOn_ = nullptr;

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	float destinationAngleY_;
	float destinationAngleX_;

	Information information_;
};

