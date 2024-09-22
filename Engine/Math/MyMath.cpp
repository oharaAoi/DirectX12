#include "MyMath.h"

float Length(const Vector3& vec3){
    return std::sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
}

Vector3 Normalize(const Vector3& vec3){
    Vector3 result = vec3;

    float len = Length(vec3);
    if (len != 0) {
        result.x /= len;
        result.y /= len;
        result.z /= len;
    }

    return result;
}

float Dot(const Vector3& v1, const Vector3& v2) {
    float result{};
    result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    Vector3 result{};
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

/// <summary>
/// スクリーン座標からワールド座標に変換する関数
/// </summary>
/// <param name="screenPos">:スクリーン上の座標</param>
/// <param name="inverseWvpMat">:逆ワールドプロジェクション行列</param>
/// <param name="setDirection">:ワールド上のどこに設定するか</param>
/// <returns>ワールド座標</returns>
Vector3 ScreenToWorldCoordinate(const Vector2& screenPos, const Matrix4x4& inverseWvpMat, const float& setDirection) {
    // ニアとファーを求める
    Vector3 posNear = Vector3(screenPos.x, screenPos.y, 0);
    Vector3 posFar = Vector3(screenPos.x, screenPos.y, 1);

    //　スクリーン座標系からワールド座標形に
    posNear = Transform(posNear, inverseWvpMat);
    posFar = Transform(posFar, inverseWvpMat);

    // ニアの点からファーの点に向かう光線を求める
    Vector3 mouseDirection = posFar - posNear;
    mouseDirection = Normalize(mouseDirection);
    
    // 結果
    Vector3 result = posNear + (mouseDirection * setDirection);

    return result;
}

/// <summary>
/// ベクトル変換
/// </summary>
/// <param name="v"></param>
/// <param name="m"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
    Vector3 result{
        v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
        v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
        v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
    };

    return result;
}
