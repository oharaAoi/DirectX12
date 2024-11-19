#include "CanThrowObject.h"

void CanThrowObject::ReadyToThrow(const Vector3& velocity) {
	velocity_ = velocity;
	isThrowed_ = true;
}
