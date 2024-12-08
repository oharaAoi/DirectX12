#pragma once
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/ParticleSystem/EffectPersistence.h"

enum class EmitterShape {
	Sphere,
	Cone,
	Box,
};

/// <summary>
/// Emitterの基底クラス
/// </summary>
class GpuEmitter {
public:

	struct CommonEmitter{
		Vector4 rotate;			// 回転(Quaternion)
		Vector3 translate;		// 位置
		uint32_t shape;			// emitterの種類
		uint32_t count;			// 射出数
		float frequency;		// 射出間隔
		float frequencyTime;	// 時間調整用
		int emit;				// 射出許可
		Vector4 color;			// 色
		float speed;			// 速度
		float pad[3];
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

	GpuEmitter() = default;
	virtual ~GpuEmitter() = default;

	virtual void Init();
	virtual void Update() ;
	virtual void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	virtual void DrawShape(const Matrix4x4& viewProjectionMat) = 0;

	void Move();

	virtual void Save();

	virtual void Load();

#ifdef _DEBUG
	virtual void Debug_Gui();
#endif

	const std::string& GetLabel() const { return label_; }

protected:

	ComPtr<ID3D12Resource> commonBuffer_;
	CommonEmitter* commonEmitter_;
	
	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

	const uint32_t kCommonParameters_ = 2;

	// 回転
	Quaternion rotate_;
	Quaternion deltaRotate_;

	std::string label_;

	EmitterParameter emitterParameter_;
	float lifeTime_;
	bool isMove_ = false;
	bool isDead_;
};

