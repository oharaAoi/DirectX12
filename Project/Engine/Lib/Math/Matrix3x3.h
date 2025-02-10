#pragma once

class Matrix3x3 final {
public:
	float m[3][3];

	//3x3の行列の積
	static Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2);
};