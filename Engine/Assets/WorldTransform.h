#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Math/Quaternion.h"


template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

struct WorldTransformData {
	Matrix4x4 matWorld;
	Matrix4x4 worldInverseTranspose;
};

class WorldTransform {
public:

	WorldTransform();
	~WorldTransform();

	void Finalize();
	void Init(ID3D12Device* device);
	void Update(const Matrix4x4& mat = MakeIdentity4x4());
	void Draw(ID3D12GraphicsCommandList* commandList) const;

	void ImGuiDraw();


	/// <summary>
	/// gltfファイルで読み込んだモデルのLoacalMatを適応させる
	/// </summary>
	/// <param name="mat"></param>
	void AdaptToGLTF(const Matrix4x4& mat) const;

public:

	void SetParent(const Matrix4x4& parentMat);

	void SetMatrix(const Matrix4x4& mat);
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetQuaternion(const Quaternion& quaternion) { rotation_ = quaternion; }
	void SetTranslation(const Vector3& translation) { translation_ = translation; }

	void SetTranslation_X(const float& x) { translation_.x = x; }
	void SetTranslation_Y(const float& y) { translation_.y = y; }
	void SetTranslation_Z(const float& z) { translation_.z = z; }

	const Vector3 GetScale() const { return scale_; }
	const Vector3 GetTranslation() const { return translation_; }
	const Quaternion GetQuaternion() const { return rotation_; }
	const Matrix4x4& GetWorldMatrix() const { return worldMat_; }

private:

	ComPtr<ID3D12Resource> cBuffer_;
	WorldTransformData* data_;

	Vector3 scale_;
	Quaternion rotation_;
	Vector3 translation_;

	Matrix4x4 worldMat_;
	const Matrix4x4* parentMat_ = nullptr;

	float test_angle_ = 0;
};

