#pragma once
#include "Engine.h"
#include "BaseParticle.h"

class Particle {
public:

	void Init(const std::string& fileName);

	void Update(const Matrix4x4& viewMat, const Matrix4x4& projection);

	void Draw();

private:

	kTransform transform_;

	std::unique_ptr<BaseParticle> particles_;

};

