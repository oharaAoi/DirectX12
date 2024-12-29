#pragma once
// engine
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Z注視するクラス
/// </summary>
class LockOn 
	: public BaseGameObject {
public:

	LockOn();
	~LockOn();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

public:

	void SetCameraMat(const Matrix4x4& cameraMat);

private:

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	Matrix4x4 cameraMat_;

};

