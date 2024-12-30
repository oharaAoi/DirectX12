#pragma once
#include "BaseLight.h"

class SpotLight
: public BaseLight {
public:

	struct SpotLightData {
		Vector4 color; // ライトの色
		Vector3 position; // ライトの位置
		float pad;
		Vector3 eyePos;
		float intensity; // 輝度
		Vector3 direction; // 方向
		float distance;
		float decay;	// 減衰
		float cosAngle; // スポットライトの余弦
		float cosFalloffStart;
		float padding[1];
	};

public :

	SpotLight();
	~SpotLight();

	void Init(ID3D12Device* device, const size_t& size) override;

	void Finalize() override;

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) override;

public:

	void ImGuiDraw();

	void SetEyePos(const Vector3& pos) { spotLightData_->eyePos = pos; }

private:

	SpotLightData* spotLightData_ = nullptr;

	float cosDegree_;
	float falloffDegree_;
	
};

