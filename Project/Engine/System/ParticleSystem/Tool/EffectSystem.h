#pragma once
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
#include "Engine/System/ParticleSystem/ParticleField.h"
#include "Engine/System/ParticleSystem/GpuEffect.h"
#include "Engine/System/ParticleSystem/Tool/EffectPersistence.h"

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

	void Emit(const std::string& name, const Vector3& pos = Vector3::ZERO());

#ifdef _DEBUG
	void Debug_Gui();

	void EditerInit(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device);
	void EditerUpdate();

	void EndEditer();

	const bool GetIsEditerFocused() const;

#endif;

public: // accessor

	void SetCameraMatrix(const Matrix4x4& cameraMat) { cameraMat_ = cameraMat; }
	void SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection);

	const bool GetIsEffectEditer() const { return isEffectEditer_; }

private:

	void ImportEmitter(const std::string& emiterName, const Vector3& pos);

private:

	EffectPersistence* effectPersistence_ = nullptr;

	// ----------- effect ----------- //
	std::list<std::unique_ptr<GpuEffect>> effectList_;

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

