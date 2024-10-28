#include "Vector3.h"

Vector3 Vector3::Normalize() const {
    float length = Length();
    if (length == 0.0f) {
        return *this;
    }
    return Vector3(x / length, y / length, z / length);
}

float Vector3::Length() const {
    return std::sqrtf(x * x + y * y + z * z);
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
    return (v2.x * v1.x) + (v2.y * v1.y) + (v2.z * v1.z);
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
    Vector3 result{};
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t) {
    Vector3 result{};
    result.x = std::lerp(start.x, end.x, t);
    result.y = std::lerp(start.y, end.y, t);
    result.z = std::lerp(start.z, end.z, t);
    return result;
}
