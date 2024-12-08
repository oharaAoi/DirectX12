#include "GpuEmitter.h"
#include "Engine/Engine.h"

void GpuEmitter::Init() {
	// Resourceの作成
	commonBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(CommonEmitter));

	commonBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(CommonEmitter));
	commonBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&commonEmitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	rotate_ = Quaternion();

	commonEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	commonEmitter_->translate = Vector3::ZERO();
	commonEmitter_->frequency = 0.5f;
	commonEmitter_->frequencyTime = 0.0f;
	commonEmitter_->count = 5;
	commonEmitter_->emit = 0;
	commonEmitter_->speed = 1.0f;
	commonEmitter_->color = Vector4(1, 1, 1, 1);

}


void GpuEmitter::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	commonEmitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (commonEmitter_->frequencyTime >= commonEmitter_->frequency) {
		commonEmitter_->frequencyTime -= commonEmitter_->frequency;
		commonEmitter_->emit = 1;
	} else {
		commonEmitter_->emit = 0;
	}

	// 回転の更新
	rotate_ = deltaRotate_.Normalize() * rotate_;
	commonEmitter_->rotate = rotate_.Normalize();
}

void GpuEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, perFrameBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, commonBuffer_->GetGPUVirtualAddress());
}


#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GpuEmitter::Debug_Gui() {
	deltaRotate_ = Quaternion();
	ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
	ImGui::DragFloat3("deltaRotate", &deltaRotate_.x, 0.01f);
	ImGui::DragFloat3("translate", &commonEmitter_->translate.x, 0.1f);
	ImGui::DragFloat("frequency", &commonEmitter_->frequency, 0.1f);
	ImGui::DragFloat("frequencyTime", &commonEmitter_->frequencyTime, 0.1f);
	ImGui::DragScalar("count", ImGuiDataType_U32, &commonEmitter_->count);
	ImGui::SliderInt("emit", &commonEmitter_->emit, 0, 1);
	ImGui::DragFloat("speed", &commonEmitter_->speed, 0.1f);
	ImGui::ColorEdit4("color", &commonEmitter_->color.x);
}
#endif