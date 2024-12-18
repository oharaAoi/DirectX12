#include "GpuEmitter.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

GpuEmitter::GpuEmitter() {}
GpuEmitter::~GpuEmitter() {
	perFrameBuffer_.Reset();
	emitterBuffer_.Reset();
	emitterBuffer_.Reset();
}

void GpuEmitter::Init() {
	// gpuに送るResourceの作成
	emitterBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(SphereEmitter));

	emitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(SphereEmitter));
	emitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&sphereEmitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	sphereEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sphereEmitter_->scale = Vector3(1.0f, 1.0f, 1.0f);
	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->frequency = 0.5f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 5;
	sphereEmitter_->emit = 0;
	sphereEmitter_->speed = 1.0f;
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->emissionType = 0;
	sphereEmitter_->lifeTime = 1.0f;
	sphereEmitter_->color = Vector4(1, 1, 1, 1);
}

void GpuEmitter::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	sphereEmitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (sphereEmitter_->frequencyTime >= sphereEmitter_->frequency) {
		sphereEmitter_->frequencyTime -= sphereEmitter_->frequency;
		sphereEmitter_->emit = 1;
	} else {
		sphereEmitter_->emit = 0;
	}

	rotate_ = deltaRotate_.Normalize() * rotate_;
	sphereEmitter_->rotate = rotate_.Normalize();
	deltaRotate_ = Quaternion();
}

void GpuEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, emitterBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, perFrameBuffer_->GetGPUVirtualAddress());
}

#ifdef _DEBUG
void GpuEmitter::Debug_Gui() {
	ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
	ImGui::DragFloat3("deltaRotate", &deltaRotate_.x, 0.01f);
	ImGui::DragFloat3("scale", &sphereEmitter_->scale.x, 0.1f);
	ImGui::DragFloat3("translate", &sphereEmitter_->translate.x, 0.1f);
	ImGui::DragFloat("frequency", &sphereEmitter_->frequency, 0.1f);
	ImGui::DragFloat("frequencyTime", &sphereEmitter_->frequencyTime, 0.1f);
	ImGui::DragScalar("count", ImGuiDataType_U32, &sphereEmitter_->count);
	ImGui::SliderInt("emit", &sphereEmitter_->emit, 0, 1);
	ImGui::DragFloat("speed", &sphereEmitter_->speed, 0.1f);
	ImGui::DragFloat("radius", &sphereEmitter_->radius, 0.1f);
	ImGui::DragScalar("emissionType", ImGuiDataType_U32, &sphereEmitter_->emissionType);
	ImGui::DragFloat("lifeTime", &sphereEmitter_->lifeTime, 0.1f);
	ImGui::ColorEdit4("color", &sphereEmitter_->color.x);
}
#endif