#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Lib/IJsonConverter.h"

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

	const Matrix4x4& GetRotateMat() const { return rotateMat_; }

private:

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	float destinationAngleY_;
	float destinationAngleX_;

	Information information_;
};

