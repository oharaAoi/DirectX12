#include "SphereEmitter.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/DrawUtils.h"

SphereEmitter::SphereEmitter() {
}

SphereEmitter::~SphereEmitter() {
	emitterBuffer_.Reset();
	perFrameBuffer_.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Init() {
	// Resourceの作成
	emitterBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(Emitter));

	emitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(Emitter));
	emitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&sphereEmitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	sphereEmitter_->translate = Vector3::ZERO();
	sphereEmitter_->radius = 1.0f;
	sphereEmitter_->frequency = 0.5f;
	sphereEmitter_->frequencyTime = 0.0f;
	sphereEmitter_->count = 10;
	sphereEmitter_->emit = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Update() {
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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コマンドリストに登録
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, emitterBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, perFrameBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::DrawShape(const Matrix4x4& viewProjectionMat) {
	DrawSphere(sphereEmitter_->translate, sphereEmitter_->radius, viewProjectionMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void SphereEmitter::Debug_Gui() {
	ImGui::DragFloat3("translate", &sphereEmitter_->translate.x, 0.1f);
	ImGui::DragFloat("radius", &sphereEmitter_->radius, 0.1f);
	ImGui::DragFloat("frequency", &sphereEmitter_->frequency, 0.1f);
	ImGui::DragFloat("frequencyTime", &sphereEmitter_->frequencyTime, 0.1f);
	ImGui::DragScalar("count", ImGuiDataType_U32, &sphereEmitter_->count);
	ImGui::SliderInt("emit", &sphereEmitter_->emit, 0, 1);
}
#endif