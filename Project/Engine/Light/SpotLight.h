#pragma once
#include "BaseLight.h"
#include <string>

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

	void AddAdjustment();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void AdaptAdjustment();

public:

	void ImGuiDraw();

	void SetEyePos(const Vector3& pos) { spotLightData_->eyePos = pos; }

	void SetDistance(float distance) { spotLightData_->distance = distance; }
	const float GetDistance() const { return spotLightData_->distance; }

private:

	SpotLightData* spotLightData_ = nullptr;

	std::string groupName_ = "spotLight";

	float cosDegree_;
	float falloffDegree_;
	
};

