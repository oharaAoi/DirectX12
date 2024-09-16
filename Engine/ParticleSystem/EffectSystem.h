#pragma once
// DirectX
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
// Emmiter
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/BaseEffect.h"
#include "Engine/ParticleSystem/EmissionEffect.h"

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

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static EffectSystem* GetInstacne();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// Effectを生成する
	/// </summary>
	void CreateEffect();

public:	// ImGui上でEffectだけを描画するための処理をする関数

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void Begin();


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
	// swapChain
	ComPtr<ID3D12Resource> swapChainRenderResource_[2];

	DirectXCommon* dxCommon_ = nullptr; 
	DescriptorHeap* descriptorHeaps_ = nullptr;
	DirectXCommands* dxCommands_ = nullptr;
};

