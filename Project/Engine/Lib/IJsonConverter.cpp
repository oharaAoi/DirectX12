#include "IJsonConverter.h"

json toJson(const Vector2& v) {
	return json{ {"x", v.x}, {"y", v.y}};
}


json toJson(const Vector3& v) {
	return json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
}


