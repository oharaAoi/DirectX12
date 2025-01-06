#include "SphereEmitter.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/DrawUtils.h"

SphereEmitter::SphereEmitter() {
}

SphereEmitter::~SphereEmitter() {
	commonBuffer_.Reset();
	perFrameBuffer_.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Init() {
	// 共通部分の初期化
	GpuEmitter::Init();

	label_ = "sphere";

	// sphere形状の初期化
	sphereEmitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(Emitter));
	sphereEmitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

	// parametrの初期化
	emitter_->radius = 1.0f;
	commonEmitter_->shape = static_cast<uint32_t>(EmitterShape::Sphere);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Update() {
	GpuEmitter::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コマンドリストに登録
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + kCommonParameters_ + commonEmitter_->shape, sphereEmitterBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::DrawShape(const Matrix4x4& viewProjectionMat) {
	DrawSphere(commonEmitter_->translate, emitter_->radius, viewProjectionMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Save() {
	GpuEmitter::Save();
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	persistence->AddItem(label_, "radius", emitter_->radius);
	persistence->Save(false, label_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void SphereEmitter::Load() {
	GpuEmitter::Load();
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	emitter_->radius = persistence->GetValue<float>(label_, "radius");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
#include <cstring>
void SphereEmitter::Debug_Gui() {
	GpuEmitter::Debug_Gui();
	ImGui::DragFloat("radius", &emitter_->radius, 0.1f);

	strncpy_s(inputFileNameBuffer_, label_.c_str(), sizeof(inputFileNameBuffer_));
	inputFileNameBuffer_[sizeof(inputFileNameBuffer_) - 1] = '\0'; // null終端を確実に設定
	if (ImGui::InputText("##effectName", inputFileNameBuffer_, sizeof(inputFileNameBuffer_))) {
		label_ = inputFileNameBuffer_; // 入力が変更された場合に更新
	}
	if (ImGui::Button("Save")) {
		Save();
	}
}
#endif