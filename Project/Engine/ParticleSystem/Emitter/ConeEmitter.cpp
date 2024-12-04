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
	// Resourceの作成
	emitterBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(Emitter));

	emitterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(Emitter));
	emitterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::Update() {
	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	emitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (emitter_->frequencyTime >= emitter_->frequency) {
		emitter_->frequencyTime -= emitter_->frequency;
		emitter_->emit = 1;
	} else {
		emitter_->emit = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コマンドリストに登録
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, emitterBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, perFrameBuffer_->GetGPUVirtualAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　形状を描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void ConeEmitter::DrawShape(const Matrix4x4& viewProjectionMat) {
	DrawSphere(emitter_->translate, emitter_->radius, viewProjectionMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void ConeEmitter::Debug_Gui() {
}
#endif