#pragma once
#include "Engine/Math/MyMatrix.h"

/// <summary>
/// 2dのカメラ
/// </summary>
class Camera2d {
public:

	Camera2d();
	~Camera2d();

	void Init();
	void Update();

#ifdef _DEBUG
	void Debug_Gui() {};
#endif // _DEBUG

private:

	Matrix4x4 projectionMatrix2D_;
	Matrix4x4 viewMatrix2D_;
	Matrix4x4 viewport2D_;

};

