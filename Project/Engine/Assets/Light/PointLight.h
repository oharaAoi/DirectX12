#pragma once
#include "BaseLight.h"

class PointLight
	: public BaseLight {
public: // メンバ構造体

	struct PointLightData {
		Vector4 color; // ライトの色
		Vector3 position; // ライトの位置
		float pad;
		Vector3 eyePos;
		float intensity; // 輝度
		float radius;	// 最大距離
		float decay;	// 減衰率
		float padding[2];
	};

public:

	PointLight();
	~PointLight();

	void Init(ID3D12Device* device, const size_t& size) override;

	void Finalize() override;

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) override;

	//======================================================
	// 
	//======================================================

	void ImGuiDraw();

	void SetEyePos(const Vector3& pos) { pointLightData_->eyePos = pos; }

private:

	PointLightData* pointLightData_ = nullptr;

};

