#pragma once
// DirectX
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
// Emmiter
#include "Engine/ParticleSystem/Emitter/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/BaseEffect.h"
#include "Engine/ParticleSystem/EffectPersistence.h"

class EffectSystemEditer;

class EffectSystem {
public:

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

#ifdef _DEBUG
	void Debug_Gui();

	void EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);

	void PostDraw();

	const bool GetIsEditerFocused() const;

#endif;

public: // accessor

	void SetCameraMatrix(const Matrix4x4& cameraMat) { cameraMat_ = cameraMat; }
	void SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection);

	const bool GetIsEffectEditer() const { return isEffectEditer_; }

private:

	const std::string kDirectoryPath_ = "./Game/Resources/GameData/Emitter/";

	EffectPersistence* effectPersistence_ = nullptr;

	std::unique_ptr<ParticleField> particleField_ = nullptr;
	
	Matrix4x4 cameraMat_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	bool isEffectEditer_ = false;

	// ------------------- ImGui上でEffectを作成するために必要な変数 ------------------- //
	D3D12_RESOURCE_BARRIER barrier_;

	DirectXCommon* dxCommon_ = nullptr; 

	std::unique_ptr<EffectSystemEditer> editer_;
};

