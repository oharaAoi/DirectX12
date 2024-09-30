#include "Emitter.h"
#include "BaseEffect.h"

Emitter::Emitter(BaseEffect* effect) : effect_(effect){
	Init();
}

Emitter::~Emitter() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Init() {
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f},{0.0f, 0.0f, 0.0f} };
	count_ = 3;
	frequency_ = 1.0f;
	frequencyTime_ = 0.0f;
	size_ = { 0.1f, 0.1f, 0.1f };
	firstVelocity_ = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	lifeTime_ = RandomFloat(4.0f, 5.0f);

	isRangeDraw_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Update() {
	frequencyTime_ += kDeltaTime_;// 時刻を進める
	if (frequency_ <= frequencyTime_) { // 頻度より大きいなら発生
		CreateParticle();// 発生処理
		frequencyTime_ = 0;// 余計に過ぎた時間も紙した頻度計算する
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Draw(const Matrix4x4& viewProjection) {
	if (isRangeDraw_) {
		Vector3 center = transform_.translate;
		Vector3 size = size_;
		std::array<Vector3, 8> point = {
			Vector3{center.x - size.x,center.y + size.y, center.z - size.z }, // front_LT
			Vector3{center.x + size.x,center.y + size.y, center.z - size.z }, // front_RT
			Vector3{center.x + size.x,center.y - size.y, center.z - size.z }, // front_RB
			Vector3{center.x - size.x,center.y - size.y, center.z - size.z }, // front_LB
			Vector3{center.x - size.x,center.y + size.y, center.z + size.z }, // back_LT
			Vector3{center.x + size.x,center.y + size.y, center.z + size.z }, // back_RT
			Vector3{center.x + size.x,center.y - size.y, center.z + size.z }, // back_RB
			Vector3{center.x - size.x,center.y - size.y, center.z + size.z }, // back_LB
		};

		for (uint32_t oi = 0; oi < 4; oi++) {
			Render::DrawLine(point[oi], point[(oi + 1) % 4], { 1,0,0,1 }, viewProjection);
			uint32_t j = oi + 4;
			Render::DrawLine(point[j], point[(j + 1) % 4 + 4], { 1,0,0,1 }, viewProjection);
			Render::DrawLine(point[oi], point[j], { 1,0,0,1 }, viewProjection);
		}
	}
}

void Emitter::Debug_Gui() {
	ImGui::Begin("Emitter");
	if (ImGui::Button("emit")) {
		CreateParticle();
	}
	ImGui::DragFloat3("translation", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("size", &size_.x, 0.01f);
	ImGui::DragScalar("count", ImGuiDataType_U32, &count_, 1);
	ImGui::Checkbox("RangeDraw", &isRangeDraw_);
	ImGui::End();
}

void Emitter::CreateParticle() {
	// 値の設定
	firstVelocity_ = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	lifeTime_ = RandomFloat(1.0f, 5.0f);

	// 生成時に必要なデータを生成する
	BaseEffect::ParticleCreateData data(transform_, size_, count_, Normalize(firstVelocity_), lifeTime_);
	// リストにパーティクルを追加する
	effect_->AddParticleList(data);
}
