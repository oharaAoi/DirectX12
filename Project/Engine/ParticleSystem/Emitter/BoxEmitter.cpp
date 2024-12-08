#include "BoxEmitter.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/DrawUtils.h"

BoxEmitter::BoxEmitter() {}
BoxEmitter::~BoxEmitter() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::Init() {
	// 共通部分の初期化
	GpuEmitter::Init();

	label_ = "box";

	// sphere形状の初期化
	boxEmitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(Emitter));
	boxEmitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

	emitter_->size_ = Vector3(1, 1, 1);

	obb_.center = commonEmitter_->translate;
	obb_.size = emitter_->size_;
	obb_.MakeOBBAxis(Quaternion::ToQuaternion(commonEmitter_->rotate));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::Update() {
	obb_.center = commonEmitter_->translate;
	obb_.size = emitter_->size_;
	obb_.MakeOBBAxis(Quaternion::ToQuaternion(commonEmitter_->rotate));

	GpuEmitter::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　commandListに積む
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + kCommonParameters_ + commonEmitter_->shape, commonBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状を描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::DrawShape(const Matrix4x4& viewProjectionMat) {
	DrawOBB(obb_, viewProjectionMat, Vector4(0,1,0,1));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::Save() {
	GpuEmitter::Save();
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	persistence->AddItem(label_, "size", emitter_->size_);
	persistence->Save(false, label_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

void BoxEmitter::Load() {
	GpuEmitter::Load();
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	emitter_->size_ = persistence->GetValue<Vector3>(label_, "size");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void BoxEmitter::Debug_Gui() {
	GpuEmitter::Debug_Gui();
	ImGui::DragFloat3("size", &emitter_->size_.x, 0.1f);

	ImGui::InputText("##effectName", &label_[0], sizeof(char) * 64);
	if (ImGui::Button("Save")) {
		Save();
	}
}
#endif