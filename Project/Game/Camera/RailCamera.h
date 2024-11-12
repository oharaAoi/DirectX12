#pragma once
#include "BaseCamera.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/Easing.h"

class RailCamera
	: public BaseCamera {
public:

	RailCamera();
	~RailCamera();

	void Init();
	void Update();
	void Draw() const;

	void RailMove();

	void ReStart();

	void SetControlPoints(const std::vector<Vector3>& points);
	void SetControlRotateZ(const std::vector<float>& points);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	WorldTransform* GetWorldTransform() { return cameraObj_->GetTransform(); }

	const uint32_t GetEyeIndex() const { return eyeIndex_; }

	void SetIsMove(bool isMove) { isMove_ = isMove; }

	const bool GetIsFinish() { return isFinish_; }

	const bool GetIsBoss() const { return isBoss_; }

private:
	// 制御点
	std::vector<Vector3> controlPoints_;
	std::vector<float> controlRotateZ_;

	float frameCount_ = 0;

	// 今の位置と目の前の位置
	uint32_t eyeIndex_ = 0;
	uint32_t forwardIndex_ = 0;

	Vector3 offset_;
	// カメラ自体がどのくらい動いているか
	Vector3 localTranslation_;
	// カメラ自体の回転
	Vector3 localRotate_;

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	float t;
	float t2;

	// ---------------------------------------
	bool isMove_;
	bool isFinish_;

	bool isBoss_;

	std::unique_ptr<BaseGameObject> cameraObj_ = nullptr;

};

