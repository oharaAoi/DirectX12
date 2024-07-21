#include "WorldTransform.h"

WorldTransform::WorldTransform() {
}

WorldTransform::~WorldTransform() {
	Finalize();
}

void WorldTransform::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(WorldTransformData));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	// 値を初期化しておく
	scale_ = {1.0f, 1.0f, 1.0f};
	rotation_ = { 0.0f, 0.0f, 0.0f };
	translation_ = { 0.0f, 0.0f, 0.0f };
}

void WorldTransform::Update() {
	// GPUに送るデータを更新
	data_->matWorld = MakeAffineMatrix({scale_, rotation_, translation_});
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
