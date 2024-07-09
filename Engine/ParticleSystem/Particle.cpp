#include "Particle.h"

void Particle::Init(const std::string& fileName, const uint32_t& particleNum) {
	kNumInstance_ = particleNum;
	particles_ = Engine::CreateBaseParticle(fileName, particleNum);
	useBillboard_ = false;

	rotate_ = { 0.0f, 0.0f, 0.0f };
}

void Particle::Update(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	liveNumInstance_ = 0;
	for (std::list<ParticleData>::iterator particleIter = particlesData_.begin(); particleIter != particlesData_.end();) {
		if (particleIter->lifeTime <= particleIter->currentTime) {
			particleIter = particlesData_.erase(particleIter);
			continue;
		}

		// 回転・移動などの処理
		particleIter->transform.rotate = rotate_;
		particleIter->transform.translate += particleIter->velocity * kDeltaTime;

		// 行列の生成
		Matrix4x4 worldMat;
		if (useBillboard_) {
			worldMat = MakeAffineMatrix(particleIter->transform) * cameraMat_;
		} else {
			worldMat = MakeAffineMatrix(particleIter->transform);
		}

		// 色の変更
		particleIter->color.w = 1.0f - (particleIter->currentTime / particleIter->lifeTime);
		// lifeCountの更新
		particleIter->currentTime += kDeltaTime;

		// パーティクルが一定数を超えないように
		if (liveNumInstance_ < kNumInstance_) {
			// 更新処理
			particles_->Update(worldMat, viewMat, projection, particleIter->color, liveNumInstance_);
			liveNumInstance_++;
		}

		// 次のパーティクルへ
		++particleIter;
	}
}


void Particle::Draw() {
	Engine::DrawParticle(particles_.get(), liveNumInstance_);
}

void Particle::ImGuiDraw() {
	ImGui::Begin("particle");
	ImGui::Text("liveCount: %d", liveNumInstance_);
	ImGui::Checkbox("useBillboard", &useBillboard_);
	ImGui::DragFloat("rotate.x", &rotate_.z, 0.1f);
	ImGui::End();
}

Particle::ParticleData Particle::MakeParticle(const Emitter::EmitterData& emitter) {
	ParticleData data{};
	data.transform.scale;

	data.transform.scale = { 1,1,1 };
	data.transform.rotate = { 0,0,0 };
	Vector3 randomTranslate = { RandomFloat(-emitter.size.x, emitter.size.x), RandomFloat(-emitter.size.y, emitter.size.y), RandomFloat(-emitter.size.z, emitter.size.z) };
	data.transform.translate = emitter.transform.translate + randomTranslate;
	// velocityの設定
	data.velocity = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	// colorの設定
	data.color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) ,1.0f};
	// lifeTimeの設定
	data.lifeTime = emitter.lifeTime;
	data.currentTime = 0;

	return data;
}

void Particle::SetCameraMatrix(const Matrix4x4& cameraMat) {
	cameraMat_ = cameraMat;
	cameraMat_.m[3][0] = 0.0f;
	cameraMat_.m[3][1] = 0.0f;
	cameraMat_.m[3][2] = 0.0f;
}

/// <summary>
/// パーティクルの生成
/// </summary>
/// <param name="emitter"></param>
/// <returns></returns>
std::list<Particle::ParticleData> Particle::Emit(const Emitter::EmitterData& emitter) {
	std::list<Particle::ParticleData> list;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		list.push_back(MakeParticle(emitter));
	}
	return list;
}

/// <summary>
/// パーティクルをリストに追加
/// </summary>
/// <param name="emitter"></param>
void Particle::AddParticleList(const Emitter::EmitterData& emitter) {
	// リストの結合を行う
	particlesData_.splice(particlesData_.end(), Emit(emitter));
}