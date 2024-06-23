#pragma once
#include "Engine.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "Camera.h"

class Metallic {
public:

	Metallic();
	~Metallic();

	void Init();

	void Update();

	void Draw();

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::vector<std::unique_ptr<Sphere>> spheres_;
	std::vector<kTransform> transforms_;

	std::vector<float> metallic_;
	std::vector<float> roughness_;

	float metallicParameter_;
	float roughnessParameter_;
};

