#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include "Engine/Utilities/DirectXUtils.h"
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

#ifdef _DEBUG
	void Debug_Gui();
	void Debug_Quaternion();
#endif

public:

	void SetParent(const Matrix4x4& parentMat);
	void SetParentTranslation(const Vector3& parentTranslation);

	void SetMatrix(const Matrix4x4& mat);
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetTranslaion(const Vector3& translation) { translation_ = translation; }
	void SetQuaternion(const Quaternion& quaternion) { rotation_ = quaternion; }

	void SetTranslationX(float x) { translation_.x = x; }
	void SetTranslationY(float y) { translation_.y = y; }
	void SetTranslationZ(float z) { translation_.z = z; }

	const Vector3 GetScale() const { return scale_; }
	const Vector3& GetTranslation() const { return translation_; }
	const Quaternion& GetQuaternion() const { return rotation_; }
	const Matrix4x4 GetWorldMatrix() const { return worldMat_; }

private:

	Vector3 scale_;
	Quaternion rotation_;
	Quaternion moveQuaternion_;
	Vector3 translation_;
	Matrix4x4 worldMat_;

	const Matrix4x4* parentMat_ = nullptr;
	const Vector3* parentTransition_ = nullptr;

	ComPtr<ID3D12Resource> cBuffer_;
	WorldTransformData* data_;

	float test_angle_ = 0;
};

