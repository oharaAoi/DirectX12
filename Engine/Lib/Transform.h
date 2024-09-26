#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/Quaternion.h"

struct kTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

Matrix4x4 MakeAffineMatrix(const kTransform& transform);