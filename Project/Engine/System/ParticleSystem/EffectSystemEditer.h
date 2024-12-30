#pragma once
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/System/ParticleSystem/ParticleField.h"
#include "Engine/System/ParticleSystem/EffectSystemCamera.h"
#include "Engine/System/ParticleSystem/GpuEffect.h"

/// <summary>
/// effectを作成する用のクラス
/// </summary>
class EffectSystemEditer {
public:

#ifdef _DEBUG

	EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);
	~EffectSystemEditer();

	void Finalize();
	void Init(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);
	void Update();
	void Draw() const;

	void Import();

	void PreBegin();
	void Begin();
	void End();

	void CreateEffect(const std::string& newName);

	void Debug_Gui();

public:

	const bool GetIsFocused() const { return isFocused_; }

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
	// device
	ID3D12Device* device_ = nullptr;
	// dsv
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	std::vector<std::string> emitterFiles_;
	std::string importFileName_;
	uint32_t importIndex_;

	int createShape_;

	bool isFocused_;	// windowが選択されているか

	// ----------- field ----------- //
	// filed
	std::unique_ptr<ParticleField> particleField_ = nullptr;

	// ----------- effect ----------- //
	std::list<std::unique_ptr<GpuEffect>> effectList_;

	// ----------- Saveに使用 ----------- //
	const std::string kDirectoryPath_ = "./Game/Resources/Effects/";
	std::vector<std::string> emitterNames_;

#endif // _DEBUG
};
