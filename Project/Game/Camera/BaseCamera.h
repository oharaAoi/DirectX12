#pragma once
#include "Engine/Lib/IJsonConverter.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Assets/WorldTransform.h"
#include "Enviroment.h"
#include "Engine/Lib/GameTimer.h"

/// <summary>
/// BaseとなるCamera
/// </summary>
class BaseCamera {
public:

	struct Parameter : public IJsonConverter {
		Quaternion rotate = Quaternion::AngleAxis(25.0f * toRadian, Vector3::RIGHT());
		Vector3 translate = Vector3(0.0f,4.0f,-8.0f);

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("rotate", rotate)
				.Add("translate", translate)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "rotate", rotate);
			fromJson(jsonData, "translate", translate);
		}
	};

public:

	BaseCamera() = default;
	virtual ~BaseCamera();

	virtual void Finalize();
	virtual void Init();
	virtual void Update();

#ifdef _DEBUG
	virtual void Debug_Gui(const std::string& label);
#endif // _DEBUG

public:	// アクセッサ

	const Matrix4x4& GetCameraMatrix() const { return cameraMatrix_; }

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	const Matrix4x4 GetVpvpMatrix() const;
	const Matrix4x4 GetVPVMatrix() const;
	const Vector3 GetWorldPosition() const;

protected:

	Parameter parameter_;

	QuaternionSRT transform_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 viewportMatrix_;

	float fovY_ = 0.45f;
	float near_ = 0.1f;
	float far_ = 100.0f;
};

