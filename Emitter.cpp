#include "Emitter.h"
#include "Particle.h"

Emitter::Emitter() {}

Emitter::~Emitter() {}

/// <summary>
/// 初期化
/// </summary>
void Emitter::Init() {
	emitterData_.transform = { {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f},{0.0f, 0.0f, 0.0f} };
	emitterData_.count = 3;
	emitterData_.frequency = 0.5f;
	emitterData_.frequencyTime = 0.0f;
	emitterData_.size = { 0.1f, 0.1f, 0.1f };
	emitterData_.firstVelocity = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	emitterData_.lifeTime = RandomFloat(1.0f, 5.0f);

	isRangeDraw_ = true;
}

/// <summary>
/// 更新
/// </summary>
void Emitter::Update() {
	emitterData_.frequencyTime += kDeltaTime;// 時刻を進める
	if (emitterData_.frequency <= emitterData_.frequencyTime) { // 頻度より大きいなら発生
		CreateParticle();// 発生処理
		emitterData_.frequencyTime -= emitterData_.frequency;// 余計に過ぎた時間も紙した頻度計算する
	}
	ImGuiDraw();
}

/// <summary>
///　描画
/// </summary>
void Emitter::Draw(const Matrix4x4& viewProjection) {
	if (isRangeDraw_) {
		Vector3 center = emitterData_.transform.translate;
		Vector3 size = emitterData_.size;
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
			Engine::DrawLine(point[oi], point[(oi + 1) % 4], { 1,1,1,1 }, viewProjection);
			uint32_t j = oi + 4;
			Engine::DrawLine(point[j], point[(j + 1) % 4 + 4], { 1,1,1,1 }, viewProjection);
			Engine::DrawLine(point[oi], point[j], { 1,1,1,1 }, viewProjection);
		}
	}
}

/// <summary>
/// ImGuiの描画
/// </summary>
void Emitter::ImGuiDraw() {
	ImGui::Begin("Emitter");
	if (ImGui::Button("emit")) {
		CreateParticle();
	}
	ImGui::DragFloat3("translation", &emitterData_.transform.translate.x, 0.01f);
	ImGui::DragFloat3("size", &emitterData_.size.x, 0.01f);
	ImGui::DragScalar("count", ImGuiDataType_U32, &emitterData_.count, 1);
	ImGui::Checkbox("RangeDraw", &isRangeDraw_);
	ImGui::End();
}

void Emitter::CreateParticle() {
	// 値の設定
	emitterData_.firstVelocity = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	emitterData_.lifeTime = RandomFloat(1.0f, 5.0f);

	particle_->AddParticleList(emitterData_);
}
