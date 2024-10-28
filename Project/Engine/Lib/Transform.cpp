#include "Transform.h"

Matrix4x4 MakeAffineMatrix(const kTransform& transform) {
    Matrix4x4 scaleMatrix{};
    Matrix4x4 rotateMatrix{};
    Matrix4x4 translateMatrix{};
    Matrix4x4 affineMatrix{};

    scaleMatrix = transform.scale.MakeScaleMat();
    rotateMatrix = transform.rotate.MakeRotateMat();
    translateMatrix = transform.translate.MakeTranslateMat();

    affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

    return affineMatrix;
}
