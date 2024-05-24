#pragma once
#include <memory>
// light
#include "Light.h"

class LightGroup {
public:

	enum LightKind {
		Directional
	};

public:

	LightGroup();
	~LightGroup();

	void Init(ID3D12Device* device);

	void Finalize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex, const LightKind& kind);

private:

	std::unique_ptr<Light> light_ = nullptr; 

};