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

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
    Vector3 result{};
    result.x = std::lerp(v1.x, v2.x, t);
    result.y = std::lerp(v1.y, v2.y, t);
    result.z = std::lerp(v1.z, v2.z, t);

    //v1 + (v2 - v1) * t;

    return result;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
    Vector3 result{};
    // なす角を求める
    float theta = Angle(v1, v2);

    // 引数のベクトルのlerpを求める
    float lerp = (1 - t) * Length(v1) + (t * Length(v2)); // s

    // 引数のベクトルの単位ベクトルを求める
    Vector3 unitV1 = Normalize(v1);    // ea
    Vector3 unitV2 = Normalize(v2);    // eb

    // ベクトルの角度を求める
    float midTheta1;
    float midTheta2;

    // なす角が0の時はそのままの角度を返すようにする
    if (theta == 0) {
        midTheta1 = 1;
        midTheta2 = 1;
    } else {
        midTheta1 = std::sin((1 - t) * theta) / std::sin(theta);
        midTheta2 = std::sin(t * theta) / std::sin(theta);
    }

    // 角度からベクトルを求める
    Vector3 midV1 = unitV1 * midTheta1;
    Vector3 midV2 = unitV2 * midTheta2;

    Vector3 mid = midV1 + midV2;

    // 正規の補完分と求めたベクトルを掛ける
    result = mid * lerp;

    return result;
}

float LerpShortAngle(const float& originalAngle, const float& targetAngle, const float& t) {
    float diff = targetAngle - originalAngle;

    // 角度を-2π~2πに補完
    diff = std::fmodf(diff, std::numbers::pi_v<float> *2.0f);
    // 角度を-π~πに補完
    if (diff > std::numbers::pi_v<float>) {
        diff = diff - (std::numbers::pi_v<float> *2.0f);

    } else if (diff < -std::numbers::pi_v<float>) {
        diff = diff + (std::numbers::pi_v<float> *2.0f);
    }


    return originalAngle + (diff * t);
}

float Angle(const Vector3& v1, const Vector3& v2) {
    float result;

    // ベクトル長さを求める
    float normV1 = Length(v1);
    float normV2 = Length(v2);

    if (normV1 == 0 || normV2 == 0) {
        return 0;
    }
    // 内積
    float dot = Dot(v1, v2);

    // cosを求める
    float cosTheta = dot / (normV1 * normV2);

    result = std::acos(cosTheta);

    return result;
}

/// <summary>///
/// CatmullRom補完
/// </summary>///
/// <param name="p0">点0の座標</param>///
/// <param name="p1">点1の座標</param>///
/// <param name="p2">点2の座標</param>///
/// <param name="p3">点3の座標</param>///
/// <param name="t">点1と点2の間で指定された座標</param>///
/// <returns>点1と点2の間で指定された座標</returns>
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
    const float s = 0.5f; // 数式に出てくる 1/2のこと

    float t2 = t * t; // tの2乗
    float t3 = t2 * t; // tの3乗

    // ベクトル値関数の式
    Vector3 e3 = (p0 * -1) + (p1 * 3) - (p2 * 3) + p3;
    Vector3 e2 = (p0 * 2) - (p1 * 5) + (p2 * 4) - p3;
    Vector3 e1 = (p0 * -1) + p2;
    Vector3 e0 = p1 * 2;

    return ((e3 * t3) + (e2 * t2) + (e1 * t) + e0) * s;
}

/// <summary>///
/// CatmullRomスプライン曲線上の座標を得る
/// </summary>///
/// <param name="points">制御点の集合</param>///
/// <param name="t">スプライン全区間の中で割合指定</param>///
/// <returns>座標</returns>
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t) {
    assert(points.size() >= 4 && "制御点は4以上必要です");
    // 区間数は制御点の数の-1
    size_t division = points.size() - 1;
    // 1区間の長さ(全体を1.0とした割合)
    float areaWidth = 1.0f / division;

    // 区間の始点を0.0f, 終点を1.0fとしたときの現在の位置
    float t2 = std::fmod(t, areaWidth) * division;
    // 下限(0.0f)と上限(1.0f)の範囲に収める
    t2 = std::clamp(t2, 0.0f, 1.0f);

    // 区間番号
    size_t index = static_cast<size_t>(t / areaWidth);
    // 区間番号が上限を超えないように収める
    index = std::clamp(static_cast<int>(index), 0, static_cast<int>(division) - 1);

    // 4点分のインデックス
    size_t index0 = index - 1;
    size_t index1 = index;
    size_t index2 = index + 1;
    size_t index3 = index + 2;

    // 始点の場合はp0とp1は同じ
    if (index == 0) {
        index0 = index1;
    }
    // 最後の区間はp3とp2は同じ
    if (index3 >= points.size()) {
        index3 = index2;
    }

    // 4点の座標
    const Vector3& p0 = points[index0];
    const Vector3& p1 = points[index1];
    const Vector3& p2 = points[index2];
    const Vector3& p3 = points[index3];

    return CatmullRomInterpolation(p0, p1, p2, p3, t2);
}

/// <summary>
/// ベジエ曲線を書く
/// </summary>
/// <param name="controlPoint">制御点がまとまった配列</param>
/// <returns></returns>
Vector3 Bezier(const std::vector<Vector3>& controlPoint, const float& t) {
    if (controlPoint.size() == 1) {
        return controlPoint[0];
    }

    std::vector<Vector3> lerpVec;
    for (size_t i = 0; i < controlPoint.size() - 1; ++i) {
        lerpVec.push_back(Lerp(controlPoint[i], controlPoint[i + 1], t));
    }

    return Bezier(lerpVec, t);
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