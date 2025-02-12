#pragma once
#include "BaseLight.h"
#include "Engine/Components/Attribute/AttributeGui.h"

class SpotLight :
	public BaseLight,
	public AttributeGui {
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

	struct Parameter : public IJsonConverter {
		Vector4 color = Vector4(1, 1, 1, 1);		// ライトの色
		Vector3 position = Vector3(0, 1, 0);
		float intensity = 1.0f; 
		Vector3 direction = Vector3(0, -1, 0);
		float distance = 5.0f;
		float decay = 2.0f;
		float cosAngle = std::cos(std::numbers::pi_v<float>);
		float cosFalloffStart = std::cos(std::numbers::pi_v<float> / 4.0f);

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("position", position)
				.Add("intensity", intensity)
				.Add("direction", direction)
				.Add("distance", distance)
				.Add("decay", decay)
				.Add("cosAngle", cosAngle)
				.Add("cosFalloffStart", cosFalloffStart)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "position", position);
			fromJson(jsonData, "intensity", intensity);
			fromJson(jsonData, "direction", direction);
			fromJson(jsonData, "decay", decay);
			fromJson(jsonData, "cosAngle", cosAngle);
			fromJson(jsonData, "cosFalloffStart", cosFalloffStart);
		}
	};

public :

	SpotLight();
	~SpotLight();

	void Init(ID3D12Device* device, const size_t& size) override;

	void Finalize() override;

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) override;

public:

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG


	void SetEyePos(const Vector3& pos) { spotLightData_->eyePos = pos; }

private:

	SpotLightData* spotLightData_ = nullptr;

	Parameter parameter_;

	float cosDegree_;
	float falloffDegree_;
	
};

