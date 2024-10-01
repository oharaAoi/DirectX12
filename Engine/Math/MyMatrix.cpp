#include "MyMatrix.h"
#include "Engine/Math/Quaternion.h"

/// <summary>
/// 加算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m1.m[row][col] + m2.m[row][col];
        }
    }

    return result;
}

/// <summary>
/// 減算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m1.m[row][col] - m2.m[row][col];
        }
    }

    return result;
}

/// <summary>
/// 積
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int oi = 0; oi < 4; oi++) {
                result.m[row][col] += m1.m[row][oi] * m2.m[oi][col];
            }
        }
    }

    return result;
}

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix4x4 Inverse(Matrix4x4 matrix) {
    Matrix4x4 result;

    // 単位行列を初期化
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // 前進消去
    for (int i = 0; i < 4; ++i) {
        // ピボットが0ならば行の入れ替えを行う
        if (fabs(matrix.m[i][i] <kEpsilon)) {
            for (int j = i + 1; j < 4; ++j) {
                if (matrix.m[j][i] != 0.0f) {
                    swapRows(matrix, i, j);
                    swapRows(result, i, j);
                    break;
                }
            }
        }

        // ピボットを1にする
        float pivot = matrix.m[i][i];
        scaleRow(matrix, i, 1.0f / pivot);
        scaleRow(result, i, 1.0f / pivot);

        // ピボット以下の要素を0にする
        for (int j = i + 1; j < 4; ++j) {
            float factor = matrix.m[j][i];
            addScaledRow(matrix, j, i, -factor);
            addScaledRow(result, j, i, -factor);
        }
    }

    // 後退代入
    for (int i = 3; i > 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
            float factor = matrix.m[j][i];
            addScaledRow(matrix, j, i, -factor);
            addScaledRow(result, j, i, -factor);
        }
    }

    return result;
}

void swapRows(Matrix4x4& matrix, int row1, int row2) {
    for (int i = 0; i < 4; ++i) {
        float temp = matrix.m[row1][i];
        matrix.m[row1][i] = matrix.m[row2][i];
        matrix.m[row2][i] = temp;
    }
}

void scaleRow(Matrix4x4& matrix, int row, float scalar) {
    for (int i = 0; i < 4; ++i) {
        matrix.m[row][i] *= scalar;
    }
}

void addScaledRow(Matrix4x4& matrix, int targetRow, int sourceRow, float scalar) {
    for (int i = 0; i < 4; ++i) {
        matrix.m[targetRow][i] += scalar * matrix.m[sourceRow][i];
    }
}

/// <summary>
/// 転置行列
/// </summary>
/// <param name="matrix"></param>
Matrix4x4 Transpose(const Matrix4x4& matrix) {
    Matrix4x4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // 行と列を入れ替える
            result.m[i][j] = matrix.m[j][i];
        }
    }

    return result;
}

/// <summary>
/// 単位行列の作成
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix4x4 MakeIdentity4x4() {
    Matrix4x4 result;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == col) {
                result.m[row][col] = 1.0f;
            }
            else {
                result.m[row][col] = 0.0f;
            }
        }
    }

    return result;
}

/// <summary>
/// 平行移動行列
/// </summary>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
    Matrix4x4 result{};

    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    result.m[3][0] = translate.x;
    result.m[3][1] = translate.y;
    result.m[3][2] = translate.z;
    result.m[3][3] = 1;

    return result;
}

/// <summary>
/// 拡縮行列
/// </summary>
/// <param name="scale"></param>
/// <returns></returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
    Matrix4x4 result{};

    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    result.m[3][3] = 1;

    return result;
}

/// <summary>
/// 回転行列
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateZMatrix(const float& radian) {
    Matrix4x4 result{};
    result.m[0][0] = std::cos(radian);
    result.m[0][1] = std::sin(radian);
    result.m[1][0] = -std::sin(radian);
    result.m[1][1] = std::cos(radian);
    result.m[2][2] = 1;
    result.m[3][3] = 1;

    return result;
}

Matrix4x4 MakeRotateXMatrix(const float& radian) {
    Matrix4x4 result{};
    result.m[0][0] = 1;
    result.m[1][1] = std::cos(radian);
    result.m[1][2] = std::sin(radian);
    result.m[2][1] = -std::sin(radian);
    result.m[2][2] = std::cos(radian);
    result.m[3][3] = 1;

    return result;
}

Matrix4x4 MakeRotateYMatrix(const float& radian) {
    Matrix4x4 result{};
    result.m[0][0] = std::cos(radian);
    result.m[0][2] = -std::sin(radian);
    result.m[1][1] = 1;
    result.m[2][0] = std::sin(radian);
    result.m[2][2] = std::cos(radian);
    result.m[3][3] = 1;

    return result;
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& radian) {
    Matrix4x4 result{};

    result = Multiply(MakeRotateXMatrix(radian.x), Multiply(MakeRotateYMatrix(radian.y), MakeRotateZMatrix(radian.z)));

    return result;
}


/// <summary>
/// 3次元アフィン変換
/// </summary>
/// <param name="scale"></param>
/// <param name="rotate"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
    Matrix4x4 scaleMatrix{};
    Matrix4x4 rotateMatrix{};
    Matrix4x4 translateMatrix{};
    Matrix4x4 affineMatrix{};

    scaleMatrix = MakeScaleMatrix(scale);
    rotateMatrix = MakeRotateXYZMatrix(rotate);
    translateMatrix = MakeTranslateMatrix(translate);

    affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

    return affineMatrix;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
    Matrix4x4 scaleMatrix{};
    Matrix4x4 rotateMatrix{};
    Matrix4x4 translateMatrix{};
    Matrix4x4 affineMatrix{};

    scaleMatrix = MakeScaleMatrix(scale);
    rotateMatrix = rotate.MakeMatrix();
    translateMatrix = MakeTranslateMatrix(translate);

    affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

    return affineMatrix;
}

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
    Vector3 result;
    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;

    return result;
}

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakeOrthograhicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip) {
    Matrix4x4 result{};
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = 1.0f / (farClip - nearClip);
    result.m[3][0] = (left + right) / (left - right);
    result.m[3][1] = (top + bottom) / (bottom - top);
    result.m[3][2] = nearClip / (nearClip - farClip);
    result.m[3][3] = 1;

    return result;
}

/// <summary>
/// 透視投影行列
/// </summary>
/// <param name="fovY"></param>
/// <param name="aspectRatio"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakePerspectiveFovMatrix(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip) {
    Matrix4x4 result{};

    float tangent = fovY / 2.0f;
    float cotangent = 1.0f / atanf(tangent);

    result.m[0][0] = (1.0f / aspectRatio) * cotangent;
    result.m[1][1] = cotangent;
    result.m[2][2] = farClip / (farClip - nearClip);
    result.m[2][3] = 1.0f;
    result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

    return result;
}

/// <summary>
/// ビューポート変換行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="minDepth"></param>
/// <param name="maxDepth"></param>
/// <returns></returns>
Matrix4x4 MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height, const float& minDepth, const float& maxDepth) {
    Matrix4x4 result{};
    result.m[0][0] = width / 2.0f;
    result.m[1][1] = -height / 2.0f;
    result.m[2][2] = maxDepth - minDepth;
    result.m[3][0] = left + (width / 2.0f);
    result.m[3][1] = top + (height / 2.0f);
    result.m[3][2] = minDepth;
    result.m[3][3] = 1.0f;

    return result;
}
