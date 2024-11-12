#include "Matrix4x4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"

Matrix4x4 Matrix4x4::Transpose() const {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// 行と列を入れ替える
			result.m[i][j] = m[j][i];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::MakeUnit() {
	Matrix4x4 result;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			if (row == col) {
				result.m[row][col] = 1.0f;
			} else {
				result.m[row][col] = 0.0f;
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::MakeAffine(const Matrix4x4& scale, const Matrix4x4& rotate, const Matrix4x4& translate) {
	return scale * rotate * translate;
}

Matrix4x4 Matrix4x4::MakeAffine(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMat = scale.MakeScaleMat();
	Matrix4x4 rotateMat = rotate.MakeRotateMat();
	Matrix4x4 translateMat = translate.MakeTranslateMat();
	return MakeAffine(scaleMat, rotateMat, translateMat);
}

Matrix4x4 Matrix4x4::MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 scaleMat = scale.MakeScaleMat();
	Matrix4x4 rotateMat = rotate.MakeMatrix();
	Matrix4x4 translateMat = translate.MakeTranslateMat();
	return MakeAffine(scaleMat, rotateMat, translateMat);
}

Matrix4x4 Matrix4x4::MakeOrthograhic(float left, float top, float right, float bottom, float nearClip, float farClip) {
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

Matrix4x4 Matrix4x4::MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip) {
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

Matrix4x4 Matrix4x4::MakeViewport(float left, float top, float width, float height, float minDepth, float maxDepth) {
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
