#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

struct kTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};