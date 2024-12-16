#pragma once
#include <string>
#include "Engine/Engine.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Utilities/DirectXUtils.h"

class MissileTrailEmitter {
public:

	struct SphereEmitter {
		Vector4 rotate;			// 回転(Quaternion)
		Vector3 translate;		// 位置
		uint32_t shape;			// emitterの種類
		uint32_t count;			// 射出数
		float frequency;		// 射出間隔
		float frequencyTime;	// 時間調整用
		int emit;				// 射出許可
		Vector4 color;			// 色
		float speed;			// 速度
		float radius;			// 半径
		uint32_t emissionType;	// 放射の種類
		float pad[2];
	};

	struct EmitterParameter {
		Vector3 velocity;	// 速度
		float speed;		// 移動速度
		float lifeTime;		// 生存時間
	};

	struct PerFrame {
		float time;
		float deltaTime;
	};

public:

	MissileTrailEmitter();
	~MissileTrailEmitter();

	void Init();
	void Update();
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

public:

	void SetEmitterPos(const Vector3& pos) { emitter_->translate = pos; }
	void SetEmitterColor(const Vector4& color) { emitter_->color = color; }

	const bool GetIsDead() const { return isDead_; }

private:

	ComPtr<ID3D12Resource> emitterBuffer_;
	SphereEmitter* emitter_;

	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

	EmitterParameter emitterParameter_;
	float lifeTime_;
	bool isMove_ = false;
	bool isDead_;

};

