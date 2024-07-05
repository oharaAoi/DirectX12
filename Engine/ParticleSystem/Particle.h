#pragma once
#include "Engine.h"
#include "MyRandom.h"
#include "BaseParticle.h"
#include "Emitter.h"

const float kDeltaTime = 1.0f / 60.0f;

class Particle {
public:

	struct ParticleData {
		kTransform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};
	
public:

	void Init(const std::string& fileName, const uint32_t& particleNum);

	void Update(const Matrix4x4& viewMat, const Matrix4x4& projection);

	void Draw();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw();

	ParticleData MakeParticle(const Emitter::EmitterData& emitter);

	/// <summary>
	/// カメラのマトリックスを取得する
	/// </summary>
	/// <param name="cameraMat"></param>
	void SetCameraMatrix(const Matrix4x4& cameraMat);

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="emitter"></param>
	/// <returns></returns>
	std::list<ParticleData> Emit(const Emitter::EmitterData& emitter);

	/// <summary>
	/// パーティクルをリストに追加
	/// </summary>
	/// <param name="emitter"></param>
	void AddParticleList(const Emitter::EmitterData& emitter);

private:

	// インスタンス数
	uint32_t kNumInstance_;
	uint32_t liveNumInstance_;
	// パーティクルの実体
	std::unique_ptr<BaseParticle> particles_;
	// パーティクルが持つパラメータ
	std::list<ParticleData> particlesData_;

	// ------------------------
	Matrix4x4 cameraMat_;

	bool useBillboard_;

	Vector3 rotate_;
};

