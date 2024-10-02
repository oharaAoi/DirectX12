#include "WorldTransform.h"
#include <Math/MyMatrix.h>

WorldTransform::WorldTransform() {}
WorldTransform::~WorldTransform() {
	Finalize();
}

void WorldTransform::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(WorldTransformData));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	// 値を初期化しておく
	scale_ = {1.0f, 1.0f, 1.0f};
	rotation_ = Quaternion(0.0f, 90, 0, 1.0f);
	translation_ = { 0.0f, 0.0f, 0.0f };
}

void WorldTransform::Update(const Matrix4x4& mat) {
	// GPUに送るデータを更新
	data_->matWorld = mat * MakeAffineMatrix(scale_, rotation_.Normalize(), translation_);
	data_->worldInverseTranspose = Transpose(Inverse(data_->matWorld));
}

void WorldTransform::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

void WorldTransform::ImGuiDraw() {
	ImGui::DragFloat3("scale", &scale_.x, 0.1f);
	ImGui::DragFloat3("rotation", &rotation_.x, 0.1f);
	ImGui::DragFloat3("translation", &translation_.x, 0.1f);
}

void WorldTransform::Finalize() {
	cBuffer_.Reset();
}

void WorldTransform::AdaptToGLTF(const Matrix4x4& mat) const {
	data_->matWorld = mat * data_->matWorld;
}

void WorldTransform::SetMatrix(const Matrix4x4& mat) {
	data_->matWorld = mat;
	data_->worldInverseTranspose = Transpose(Inverse(data_->matWorld));
}
