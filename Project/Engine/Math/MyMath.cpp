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

/// <summary>
/// ベジエ曲線を書く
/// </summary>
/// <param name="controlPoint">制御点がまとまった配列</param>
/// <returns></returns>
Vector3 Bezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float& t){

    Vector3 lerpVec[2]{};
    lerpVec[0] = (v2 - v1).Normalize() * Length(v2 - v1) * t;
    lerpVec[1] = (v3 - v2).Normalize() * Length(v3 - v2) * t;
    return (lerpVec[1] - lerpVec[0]).Normalize() * Length(lerpVec[1] - lerpVec[0]) * t;
}


uint32_t Red(uint32_t color){ return (color >> 24) & 0xFF; }
uint32_t Green(uint32_t color){ return (color >> 16) & 0xFF; }
uint32_t Blue(uint32_t color){ return (color >> 8) & 0xFF; }
uint32_t Alpha(uint32_t color){ return color & 0xFF; }


// RGBA形式のカラーコードをVector4形式に変換する関数 (各要素は0~1に収まる)
Vector4 FloatColor(uint32_t color){
    float delta = 1.0f / 255.0f;

    Vector4 colorf = {
        float(Red(color)) * delta,
        float(Green(color)) * delta,
        float(Blue(color)) * delta,
        float(Alpha(color)) * delta
    };

    return colorf;
}


uint32_t IntColor(const Vector4& color){
    uint32_t red = std::clamp(int(color.x * 255.0f), 0, 255) << 24;
    uint32_t green = std::clamp(int(color.y * 255.0f), 0, 255) << 16;
    uint32_t blue = std::clamp(int(color.z * 255.0f), 0, 255) << 8;
    uint32_t alpha = std::clamp(int(color.w * 255.0f), 0, 255);

    return red + green + blue + alpha;
}

uint32_t HSV_to_RGB(float h, float s, float v, float alpha){

    // 彩度が0なので明度のみを反映
    if(s == 0.0) {
        return IntColor(Vector4(v, v, v, alpha));
    }

    h *= 6.0;
    int i = int(h);
    float f = h - i;

    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    if(i % 6 == 0) {
        return  IntColor(Vector4(v, t, p, alpha));
    } else if(i % 6 == 1) {
        return  IntColor(Vector4(q, v, p, alpha));
    } else if(i % 6 == 2) {
        return  IntColor(Vector4(p, v, t, alpha));
    } else if(i % 6 == 3) {
        return  IntColor(Vector4(p, q, v, alpha));
    } else if(i % 6 == 4) {
        return  IntColor(Vector4(t, p, v, alpha));
    } else {
        return  IntColor(Vector4(v, p, q, alpha));
    }
}