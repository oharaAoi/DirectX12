#pragma once
// DirectX
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
// Emmiter
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/BaseEffect.h"
#include "Engine/ParticleSystem/EmissionEffect.h"

class EffectSystemEditer;

class EffectSystem {
public:

	/// <summary>
	/// Effectのデータ(EmitterとEffect)
	/// </summary>
	struct EffectData {
		std::list<std::unique_ptr<Emitter>> emitterList;
		std::list<std::unique_ptr<BaseEffect>> effectList;
	};
	
public:

	EffectSystem() = default;
	~EffectSystem();
	EffectSystem(const EffectSystem&) = delete;
	const EffectSystem& operator=(const EffectSystem&) = delete;

	static EffectSystem* GetInstacne();

	void Init();
	void Finalize();
	void Update();
	void Draw() const;

	/// <summary>
	/// Effectを生成する
	/// </summary>
	void CreateEffect();

#ifdef _DEBUG
	void EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);

public:	// ImGui上でEffectだけを描画するための処理をする関数

	void BeginEditer();
	void EndEditer();

	void UpdateEditer();
	void DrawEditer();

#endif;

public: // accessor

	void SetCameraMatrix(const Matrix4x4& cameraMat) { cameraMat_ = cameraMat; }
	void SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection);

private:

	std::unique_ptr<ParticleField> particleField_ = nullptr;
	std::list<EffectData> effectList_;

	Matrix4x4 cameraMat_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// ------------------- ImGui上でEffectを作成するために必要な変数 ------------------- //
	D3D12_RESOURCE_BARRIER barrier_;

	DirectXCommon* dxCommon_ = nullptr; 

	std::unique_ptr<EffectSystemEditer> editer_;
};
