#pragma once
#include <memory>
// light
#include "Light.h"
#include "Phong.h"

class LightGroup {
public:

	enum LightKind {
		Directional,
		PhongLight
	};

public:

	LightGroup();
	~LightGroup();

	void Init(ID3D12Device* device);

	void Finalize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex, const LightKind& kind);

public:

	void SetEyePos(const Vector3& pos) { eyePos_ = pos; }

private:

	std::unique_ptr<Light> light_ = nullptr; 
	std::unique_ptr<Phong> phong_ = nullptr;

	Vector3 eyePos_;
};