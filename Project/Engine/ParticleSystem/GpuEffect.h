#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/ParticleSystem/GpuParticle.h"
#include "Engine/ParticleSystem/GpuEmitter.h"

class GpuEffect {
public:

	GpuEffect();
	~GpuEffect();

	void Init();
	void Update();
	void Draw() const;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	void SetViewProjectionMat(const Matrix4x4& mat) { viewProjectionMat_ = mat; }

	/// <summary>
	/// effectの名前を登録する
	/// </summary>
	/// <param name="effectName">: effectの名前を登録する</param>
	void SetEffectName(const std::string& effectName) { effectName_ = effectName; }

private:

	std::unique_ptr<GpuParticle> gpuParticle_;
	std::unique_ptr<GpuEmitter> gpuEmitter_;

	Matrix4x4 viewProjectionMat_;

	// effectの名前
	std::string effectName_ = "";
};

