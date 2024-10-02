#pragma once
#include "DirectXMath.h"
#include <cmath>

//class Vector3;

class Matrix4x4 final {
public:

	float m[4][4];

	//*======================================================================
	//	加算
	//*======================================================================
	Matrix4x4 operator+(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result.m[row][col] = this->m[row][col] + obj.m[row][col];
			}
		}

		return result;
	}

	//*======================================================================
	//	減算
	//*======================================================================
	Matrix4x4 operator-(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result.m[row][col] = this->m[row][col] - obj.m[row][col];
			}
		}

		return result;
	}

	//*======================================================================
	//	掛算
	//*======================================================================
	Matrix4x4 operator*(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				for (int oi = 0; oi < 4; oi++) {
					result.m[row][col] += this->m[row][oi] * obj.m[oi][col];
				}
			}
		}

		return result;
	}

	Matrix4x4& operator*=(const Matrix4x4& other) {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m[i][k] * other.m[k][j];
				}
			}
		}
		*this = result;
		return *this;
	}

	Matrix4x4 Inverse() const {
		DirectX::XMMATRIX thisMat = MatrixToXMMATRIX();
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(thisMat);
		Matrix4x4 inverse;
		inverse.XMMATRIXToMatrix(DirectX::XMMatrixInverse(&det, thisMat));
		return  inverse;
	}

	DirectX::XMMATRIX MatrixToXMMATRIX()const {
		return DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
	}

	Matrix4x4* XMMATRIXToMatrix(const DirectX::XMMATRIX& xmmat) {
		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(this), xmmat);
		return this;
	}

	/*Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, const float& angle) {
		Matrix4x4 result;

		float cosf = std::cosf(angle);
		float sinf = std::sinf(angle);

		result.m[0][0] = ((axis.x * axis.x) * (1 - cosf)) + cosf;
		result.m[0][1] = ((axis.x * axis.y) * (1 - cosf)) + (sinf * axis.z);
		result.m[0][2] = ((axis.x * axis.z) * (1 - cosf)) + (sinf * axis.y);
		result.m[0][3] = 0.0f;

		result.m[1][0] = ((axis.x * axis.y) * (1 - cosf)) + (sinf * axis.z);
		result.m[1][1] = ((axis.y * axis.y) * (1 - cosf)) + cosf;
		result.m[1][2] = ((axis.y * axis.z) * (1 - cosf)) + (sinf * axis.y);
		result.m[1][3] = 0.0f;


		return result;
	}*/
};
