#include "MyMatrix.h"
#include <stdexcept>
#include "Engine/Lib/Math/Quaternion.h"

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
/// 逆行列を計算する関数
/// </summary>
/// <param name="matrix">元の行列</param>
/// <returns>逆行列</returns>
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
        // ピボットが0に近い場合は行の入れ替えを行う
        if (fabs(matrix.m[i][i]) < kEpsilon) {
            bool swapped = false;
            for (int j = i + 1; j < 4; ++j) {
                if (fabs(matrix.m[j][i]) > kEpsilon) {
                    swapRows(matrix, i, j);
                    swapRows(result, i, j);
                    swapped = true;
                    break;
                }
            }
            // もし入れ替えが行われなければ、逆行列は存在しない（特異行列）
            if (!swapped) {
                throw std::runtime_error("Singular matrix, cannot find inverse.");
            }
        }

        // ピボットを1に正規化
        float pivot = matrix.m[i][i];
        scaleRow(matrix, i, 1.0f / pivot);
        scaleRow(result, i, 1.0f / pivot);

        // ピボット以下の行を0にする（前進消去）
        for (int j = i + 1; j < 4; ++j) {
            float factor = matrix.m[j][i];
            addScaledRow(matrix, j, i, -factor);
            addScaledRow(result, j, i, -factor);
        }
    }

    // 後退代入
    for (int i = 3; i >= 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
            float factor = matrix.m[j][i];
            addScaledRow(matrix, j, i, -factor);
            addScaledRow(result, j, i, -factor);
        }
    }

    return result;
}

/// <summary>
/// 2つの行を入れ替える関数
/// </summary>
/// <param name="matrix">操作対象の行列</param>
/// <param name="row1">入れ替える行1</param>
/// <param name="row2">入れ替える行2</param>
void swapRows(Matrix4x4& matrix, int row1, int row2) {
    for (int i = 0; i < 4; ++i) {
        float temp = matrix.m[row1][i];
        matrix.m[row1][i] = matrix.m[row2][i];
        matrix.m[row2][i] = temp;
    }
}

/// <summary>
/// 行全体にスカラー値をかける関数
/// </summary>
/// <param name="matrix">操作対象の行列</param>
/// <param name="row">操作対象の行</param>
/// <param name="scalar">スカラー値</param>
void scaleRow(Matrix4x4& matrix, int row, float scalar) {
    for (int i = 0; i < 4; ++i) {
        matrix.m[row][i] *= scalar;
    }
}

/// <summary>
/// 行全体に別の行をスケールして加える関数
/// </summary>
/// <param name="matrix">操作対象の行列</param>
/// <param name="targetRow">操作対象の行</param>
/// <param name="sourceRow">加える行</param>
/// <param name="scalar">スケール値</param>
void addScaledRow(Matrix4x4& matrix, int targetRow, int sourceRow, float scalar) {
    for (int i = 0; i < 4; ++i) {
        matrix.m[targetRow][i] += scalar * matrix.m[sourceRow][i];
    }
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

    //assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;

    return result;
}

Vector3 GetEulerAnglesFromRotationMat(const Matrix4x4& R) {
    Vector3 angles;

    // Pitch (x軸の回転)
    angles.x = std::asin(-R.m[2][0]);

    // ジンバルロックのチェック
    if (std::cos(angles.x) > 0.0001) {
        // Pitchが ±90 度でない場合 (ジンバルロックではない)
        angles.z = std::atan2(R.m[2][1], R.m[2][2]); // Roll (z軸の回転)
        angles.y = std::atan2(R.m[1][0], R.m[0][0]); // Yaw (y軸の回転)
    } else {
        // Pitchが ±90 度の場合 (ジンバルロック)
        angles.z = 0.0f; // Rollは定義できないので0にする
        angles.y = std::atan2(-R.m[0][1], R.m[1][1]); // Yawの特別なケース
    }

    return angles;
}