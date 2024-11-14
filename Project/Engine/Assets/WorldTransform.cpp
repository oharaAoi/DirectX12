#include "WorldTransform.h"
#include <Math/MyMatrix.h>

WorldTransform::WorldTransform() {}
WorldTransform::~WorldTransform() {
	Finalize();
}

void WorldTransform::Finalize() {
	cBuffer_.Reset();
	data_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldTransform::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(WorldTransformData));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	// 値を初期化しておく
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotation_ = Quaternion();
	translation_ = { 0.0f, 0.0f, 0.0f };
	worldMat_ = Matrix4x4::MakeUnit();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldTransform::Update(const Matrix4x4& mat) {
	Vector3 worldTranslate = Vector3::ZERO();
	Quaternion worldRotate = Quaternion();

	rotation_ = (rotation_.Normalize() * moveQuaternion_.Normalize());
	rotation_ = rotation_.Normalize();
	moveQuaternion_ = Quaternion();

	// -------------------------------------------------
	// ↓ 平行成分の親子関係があるかを確認
	// -------------------------------------------------
	if (parentTransition_ != nullptr) {
		worldTranslate = translation_ + *parentTransition_;
	} else {
		worldTranslate = translation_;
	}

	// -------------------------------------------------
	// ↓ 回転成分の親子関係があるかを確認
	// -------------------------------------------------
	if (parentRotate_ != nullptr) {
		worldRotate = *parentRotate_ * rotation_;
	} else {
		worldRotate = rotation_;
	}

	// -------------------------------------------------
	// ↓ world行列を生成
	// -------------------------------------------------
	worldMat_ = mat * Matrix4x4::MakeAffine(scale_, worldRotate, worldTranslate);
	if (parentMat_ != nullptr) {
		worldMat_ *= *parentMat_;
	}

	// GPUに送るデータを更新
	data_->matWorld = worldMat_;
	data_->worldInverseTranspose = (worldMat_).Inverse().Transpose();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コマンドリストに送る
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldTransform::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void WorldTransform::Debug_Gui() {
	if (ImGui::TreeNode("Transform")) {
		if (ImGui::TreeNode("scale")) {
			ImGui::DragFloat3("scale", &scale_.x, 0.1f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("rotate")) {
			Debug_Quaternion();
			float norm = std::sqrtf(Quaternion::Dot(rotation_, rotation_));
			ImGui::Text("norm: %f", norm);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("translation")) {
			ImGui::DragFloat3("translation", &translation_.x, 0.1f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void WorldTransform::Debug_Quaternion() {
	ImGui::DragFloat4("rotation", &rotation_.x, 0.1f);
	ImGui::DragFloat4("moveQuaternion", &moveQuaternion_.x, 0.1f);
	if (ImGui::Button("Reset")) {
		rotation_ = Quaternion();
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Setter系
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldTransform::SetParent(const Matrix4x4& parentMat) {
	parentMat_ = &parentMat;
}

void WorldTransform::SetParentTranslation(const Vector3& parentTranslation) {
	parentTransition_ = &parentTranslation;
}

void WorldTransform::SetParentRotate(const Quaternion& parentQuaternion) {
	parentRotate_ = &parentQuaternion;
}

void WorldTransform::SetMatrix(const Matrix4x4& mat) {
	data_->matWorld = mat;
	data_->worldInverseTranspose = Inverse(data_->matWorld).Transpose();
}
