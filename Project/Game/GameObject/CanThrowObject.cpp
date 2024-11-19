#include "CanThrowObject.h"

void CanThrowObject::SetThrow(const Vector3& velocity) {
	velocity_ = velocity;
	isThrowed_ = true;
}
