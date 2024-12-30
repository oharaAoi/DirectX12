#pragma once
#include "BaseLight.h"

class DirectionalLight
: public BaseLight {
public: // メンバ構造体

	struct DirectionalLightData {
		Vector4 color; // ライトの色
		Vector3 direction; // 方向
		float pad;
		Vector3 eyePos;
		float intensity; // 輝度
		float limPower; // リムライトの強さ
	};

public:

	DirectionalLight();
	~DirectionalLight();

	void Init(ID3D12Device* device, const size_t& size) override;

	void Finalize() override;

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) override;

public:

	void ImGuiDraw();

	void SetEyePos(const Vector3& pos) { directionalLightData_->eyePos = pos; }

private:

	DirectionalLightData* directionalLightData_;

};