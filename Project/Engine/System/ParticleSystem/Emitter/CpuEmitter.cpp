#include "CpuEmitter.h"

CpuEmitter::CpuEmitter() {}
CpuEmitter::~CpuEmitter() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuEmitter::Init() {
	// -------------------------------------------------
	// ↓ Emitterのz情報
	// -------------------------------------------------
	commonEmitter_.rotate = Vector4();
	commonEmitter_.translate = Vector3();
	commonEmitter_.frequency = 0.5f;
	commonEmitter_.frequencyTime = 0.0f;
	commonEmitter_.count = 10;
	commonEmitter_.emit = 0;

	// -------------------------------------------------
	// ↓ 生成するparticleの情報
	// -------------------------------------------------
	commonEmitter_.color = Vector4(1, 1, 1, 1);
	commonEmitter_.minScale = Vector3(1, 1, 1);
	commonEmitter_.maxScale = Vector3(1, 1, 1);
	commonEmitter_.speed = 1.0f;
	commonEmitter_.lifeTime = 2.0f;
	commonEmitter_.gravity = 0.0f;
	commonEmitter_.dampig = 1.0f;

	// -------------------------------------------------
	// ↓ EmitterのAnimationの情報
	// -------------------------------------------------
	emitterParametr_.particleModel = "cube.obj";
	emitterParametr_.velocity = Vector3();
	emitterParametr_.speed = 1.0f;
	emitterParametr_.lifeTime = 1.0f;
	emitterParametr_.oneShot = false;

	isDead_ = false;
	isMove_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuEmitter::Update() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CpuEmitter::DrawShape() {

}
