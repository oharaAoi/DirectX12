#pragma once
#include "DirectXMath.h"

struct Matrix4x4 final {
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
};
