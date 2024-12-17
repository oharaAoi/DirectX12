#pragma once
#include <string>
#include "Engine/Engine.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Utilities/DirectXUtils.h"

class GpuEmitter {
public:

	struct SphereEmitter {
		Vector4 rotate;			// 回転(Quaternion)
		Vector3 scale;			// 拡縮
		float pad1;
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
		float lifeTime;			// particleのlifeTime
		float pad[1];
	};

	struct PerFrame {
		float time;
		float deltaTime;
	};

	struct EmitterParameter {
		Vector3 velocity;	// 速度
		float speed;		// 移動速度
		float lifeTime;		// 生存時間
	};

public:

	GpuEmitter();
	~GpuEmitter();

	virtual void Init();
	virtual void Update();
	virtual void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	void SetEmitterPos(const Vector3& pos) { sphereEmitter_->translate = pos; }
	void SetEmitterRotate(const Quaternion& q) { sphereEmitter_->rotate = q; }
	void SetEmitterColor(const Vector4& color) { sphereEmitter_->color = color; }

	const bool GetIsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

protected:

	ComPtr<ID3D12Resource> emitterBuffer_;
	SphereEmitter* sphereEmitter_;

	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

	EmitterParameter emitterParameter_;

	// 回転
	Quaternion rotate_;
	Quaternion deltaRotate_;

	bool isMove_ = false;
	bool isDead_ = false;
};

