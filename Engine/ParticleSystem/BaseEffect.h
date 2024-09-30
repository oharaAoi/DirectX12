#pragma once
#include "Engine.h"
#include "Engine/Math/MyRandom.h"
#include "Emitter.h"
#include "Engine/Lib/Transform.h"

/// <summary>
/// Effectの元となるクラス
/// </summary>
class BaseEffect {
public:

	/// <summary>
	/// Particleが持つデータ
	/// </summary>
	struct ParticleData {
		kTransform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	/// <summary>
	/// Particleを生成時に必要なデータ
	/// </summary>
	struct ParticleCreateData {
		const kTransform transform;	// 場所
		Vector3 size;			// エミッターのサイズ
		uint32_t count;			// 生成する個数
		Vector3 firstVelocity;	// 初速度
		float lifeTime;			// 生存期間

		ParticleCreateData(const kTransform& transformData, const Vector3& sizeData, const uint32_t& countData, const Vector3& firstVelocityData, const float& lifeTimeData) :
			transform(transformData),
			size(sizeData),
			count(countData),
			firstVelocity(firstVelocityData),
			lifeTime(lifeTimeData) {
		}
		
	};

public:

	BaseEffect() = default;
	virtual ~BaseEffect();

	virtual void Finalize();
	virtual void Init(const std::string& directoryPath, const std::string& fileName, const uint32_t& particleNum);
	virtual void Update(const Matrix4x4& viewMat, const Matrix4x4& projection);
	virtual void Draw();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	ParticleData MakeParticle(const ParticleCreateData& emitter);
	void SetCameraMatrix(const Matrix4x4& cameraMat);

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="emitter"></param>
	/// <returns></returns>
	std::list<ParticleData> Emit(const ParticleCreateData& emitter);

	/// <summary>
	/// パーティクルをリストに追加
	/// </summary>
	/// <param name="emitter"></param>
	void AddParticleList(const ParticleCreateData& emitter);

public:

	std::list<ParticleData>& GetParticleList() { return particlesData_; }

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

