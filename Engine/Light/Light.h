#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
// DirectX
#include "DirectXUtils.h"
// math
#include "MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class Light {
public: // メンバ構造体

	struct DirectionalLight {
		Vector4 color; // ライトの色
		Vector3 direction; // 方向
		float pad;
		Vector3 eyePos;
		float intensity; // 輝度
	};

public:

	Light();
	~Light();

	void Init(ID3D12Device* device);

	void Finalize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);

	void ImGuiDraw();

	void SetEyePos(const Vector3& pos) { DirectionalLightData_->eyePos = pos; }

private:

	Comptr<ID3D12Resource> lightBuffer_;
	DirectionalLight* DirectionalLightData_;

};