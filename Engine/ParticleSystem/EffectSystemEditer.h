#pragma once
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/BaseEffect.h"
#include "EffectSystemCamera.h"
#include "Engine/Utilities/DrawUtils.h"

#ifdef _DEBUG
#include <Externals/nlohmann/json.hpp>
#endif // _DEBUG


/// <summary>
/// effectを作成する用のクラス
/// </summary>
class EffectSystemEditer {
public:

#ifdef _DEBUG

	/// <summary>
	/// Effectのデータ(EmitterとEffect)
	/// </summary>
	struct EffectData {
		std::list<std::unique_ptr<Emitter>> emitterList;
		std::list<std::unique_ptr<BaseEffect>> effectList;
	};

	EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);
	~EffectSystemEditer();

	void Finalize();
	void Init(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);
	void Update();
	void Draw() const;

	void CreateEffect();

	void Begin();
	void End();

	void LoadEffectFile(const std::string& filePath);
	void SaveEffectFile(const std::string& filePath);

private:

	int32_t kClientWidth_ = 1280;
	int32_t kClientHeight_ = 720;

	std::unique_ptr<EffectSystemCamera> effectSystemCamera_ = nullptr;

	// ----------- Editerで使用する変数 ----------- //
	// RenderTarget
	RenderTarget* renderTarget_ = nullptr;
	// descriptorHeap
	DescriptorHeap* descriptorHeaps_ = nullptr;
	// commands
	DirectXCommands* dxCommands_ = nullptr;

	ID3D12Device* device_ = nullptr;

	// dsv
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	std::unique_ptr<ParticleField> particleField_ = nullptr;
	std::list<EffectData> effectList_;

#endif // _DEBUG
};
