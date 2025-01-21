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

	struct Paramter : public IJsonConverter {
		Vector4 color = Vector4(1,1,1,1);		// ライトの色
		Vector3 position = Vector3(0,1,0);	// ライトの位置
		float intensity = 1.0f;	// 輝度
		float radius = 5.0f;		// 範囲
		float decay = 0.5f;		// 減衰率

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("position", position)
				.Add("intensity", intensity)
				.Add("radius", radius)
				.Add("decay", decay)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "position", position);
			fromJson(jsonData, "intensity", intensity);
			fromJson(jsonData, "radius", radius);
			fromJson(jsonData, "decay", decay);
		}
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

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetEyePos(const Vector3& pos) { pointLightData_->eyePos = pos; }

private:

	PointLightData* pointLightData_ = nullptr;

	Paramter parameter_;
};

