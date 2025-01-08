#include "Matrix3x3.h"

Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2) {
	Matrix3x3 result{};
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			for (int i = 0; i < 3; i++) {
				result.m[row][col] += matrix1.m[row][i] * matrix2.m[i][col];
			}
		}
	}

	return result;
}
