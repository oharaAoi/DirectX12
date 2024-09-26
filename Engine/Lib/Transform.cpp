#include "Transform.h"

Matrix4x4 MakeAffineMatrix(const kTransform& transform) {
    Matrix4x4 scaleMatrix{};
    Matrix4x4 rotateMatrix{};
    Matrix4x4 translateMatrix{};
    Matrix4x4 affineMatrix{};

    scaleMatrix = MakeScaleMatrix(transform.scale);
    rotateMatrix = MakeRotateXYZMatrix(transform.rotate);
    translateMatrix = MakeTranslateMatrix(transform.translate);

    affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

    return affineMatrix;
}
