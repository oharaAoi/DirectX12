#pragma once
#include <memory>
#include "Engine/System/ParticleSystem/Emitter/GpuEmitter.h"

class GpuEffect {
public:

	GpuEffect();
	~GpuEffect();

	void Init(const EmitterShape& shape);
	void Update();
	void Draw() const;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	void CheckParticlesAlpha();

public:

	void SetViewProjectionMat(const Matrix4x4& mat) { viewProjectionMat_ = mat; }

	// Effectの名前を取得する・設定する
	const std::string& GetEffectName() const { return effectName_; }
	void SetEffectName(const std::string& effectName) { effectName_ = effectName; }

	const std::string& GetEmitterLabel() const { return gpuEmitter_->GetLabel(); }

	void SetEmitter(const std::string& effectName) const { return gpuEmitter_->SetEmitter(effectName); }
	void SetEmitterPos(const Vector3& pos) const { return gpuEmitter_->SetEmitterPos(pos); }
	void SetEmitterColor(const Vector4& color) const { return gpuEmitter_->SetEmitterColor(color); }

	// effectの終了を取得と設定
	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

private:


	std::unique_ptr<GpuEmitter> gpuEmitter_;

	Matrix4x4 viewProjectionMat_;

	// effectの名前
	std::string effectName_ = "";

	bool isAlive_ = true;
	bool allAlphaZero_ = false;
};

