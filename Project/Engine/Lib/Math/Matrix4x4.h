#pragma once
#include "DirectXMath.h"

class Vector3;
class Quaternion;

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

	// =============================================
	// 数学用関数
	// =============================================

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <returns></returns>
	Matrix4x4 Transpose() const;

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <returns></returns>
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

	/// <summary>
	/// 単位行列の作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeUnit();

	/// <summary>
	/// Affine行列を作成する
	/// </summary>
	/// <param name="scale">: 拡縮行列</param>
	/// <param name="rotate">: 回転行列</param>
	/// <param name="translate">: 平行行列</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffine(const Matrix4x4& scale, const Matrix4x4& rotate, const Matrix4x4& translate);

	/// <summary>
	/// Affine行列を作成する
	/// </summary>
	/// <param name="scale">: 拡縮</param>
	/// <param name="rotate">: 回転</param>
	/// <param name="translate">: 平行</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffine(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// Affine行列を作成する
	/// </summary>
	/// <param name="scale">: 拡縮</param>
	/// <param name="rotate">: 回転</param>
	/// <param name="translate">: 平行</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	/// <summary>
	/// 正射影行列
	/// </summary>
	/// <param name="left">:</param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	static Matrix4x4 MakeOrthograhic(float left, float top, float right, float bottom, float nearClip, float farClip);

	/// <summary>
	/// 透視投影行列
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	static Matrix4x4 MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip);

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
	static Matrix4x4 MakeViewport(float left, float top, float width, float height, float minDepth = 0, float maxDepth = 1);
};
