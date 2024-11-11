#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Engine.h"

class GpuParticle {
public:	// メンバ構造体

	struct Particle {
		Vector3 scale;		// 拡縮
		Vector3 translate;	// 座標	
		Vector3 velocity;	// 速度
		float lifeTime;		// 生存時間
		float currentTime;	// 現在の時間
		Vector4 color;		// 色
	};

public:

	GpuParticle();
	~GpuParticle();

	void Init();
	void Update();
	void Draw();

private:

	ComPtr<ID3D12Resource> particleBuffer_;

};

