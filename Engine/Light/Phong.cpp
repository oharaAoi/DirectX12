#include "Phong.h"

Phong::Phong() {
}

Phong::~Phong() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////////////////////////////////////////////////////
void Phong::Init(ID3D12Device* device) {
	lightDate_ = nullptr;
	constBuffer_ = CreateBufferResource(device, sizeof(PhongDate));
	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&lightDate_));

	lightDate_->color = { 1.0f,1.0f, 1.0f, 1.0f };
	lightDate_->lightDirection = { -1.0f, -1.0f, 1.0f };
	lightDate_->intensity_ = 1.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 終了関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Phong::Finalize() {
	constBuffer_.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Phong::Update() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// lightのデータを送る
//////////////////////////////////////////////////////////////////////////////////////////////////
void Phong::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuffer_->GetGPUVirtualAddress());
}
