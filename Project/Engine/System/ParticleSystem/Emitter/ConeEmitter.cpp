#include "ConeEmitter.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/DrawUtils.h"

ConeEmitter::ConeEmitter() {
}

ConeEmitter::~ConeEmitter() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::Init() {
	// 共通部分の初期化
	GpuEmitter::Init();

	label_ = "cone";

	// sphere形状の初期化
	coneEmitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(Emitter));
	coneEmitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

	// parametrの初期化
	emitter_->radius = 1.0f;
	emitter_->height = 1.0f;
	emitter_->angle = 2.0f;

	commonEmitter_->shape = static_cast<uint32_t>(EmitterShape::Cone);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::Update() {
	GpuEmitter::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コマンドリストに登録
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	GpuEmitter::BindCmdList(commandList, rootParameterIndex);
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + kCommonParameters_ + commonEmitter_->shape, coneEmitterBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状を描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::DrawShape(const Matrix4x4& viewProjectionMat) {
	Quaternion rotate = Quaternion(commonEmitter_->rotate.x, commonEmitter_->rotate.y, commonEmitter_->rotate.z, commonEmitter_->rotate.w);
	DrawCone(commonEmitter_->translate, rotate, emitter_->radius, emitter_->angle, emitter_->height, viewProjectionMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::Save() {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	GpuEmitter::Save();
	persistence->AddItem(label_, "radius", emitter_->radius);
	persistence->AddItem(label_, "angle", emitter_->angle);
	persistence->AddItem(label_, "height", emitter_->height);
	persistence->Save(false, label_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::Load() {
	GpuEmitter::Load();
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	emitter_->radius = persistence->GetValue<float>(label_, "radius");
	emitter_->angle = persistence->GetValue<float>(label_, "angle");
	emitter_->height = persistence->GetValue<float>(label_, "height");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void ConeEmitter::Debug_Gui() {
	GpuEmitter::Debug_Gui();
	ImGui::DragFloat("radius", &emitter_->radius, 0.1f);
	ImGui::DragFloat("angle", &emitter_->angle, 0.1f);
	ImGui::DragFloat("height", &emitter_->height, 0.1f);

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