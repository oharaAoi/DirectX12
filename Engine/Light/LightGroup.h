#pragma once
#include <memory>
// light
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Phong.h"

class LightGroup {
public:

	enum LightKind {
		Directional,
		PBR,
	};

public:

	LightGroup();
	~LightGroup();

	void Init(ID3D12Device* device);

	void Finalize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);
	void DrawPar(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);

	void DrawLi(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);

public:

	void SetEyePos(const Vector3& pos) { eyePos_ = pos; }

private:

	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;
	std::unique_ptr<PointLight> pointLight_ = nullptr;
	std::unique_ptr<SpotLight> spotLight_ = nullptr;
	std::unique_ptr<Phong> phong_ = nullptr;

	Vector3 eyePos_;
};