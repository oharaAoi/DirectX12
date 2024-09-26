#pragma once
#include "BaseEffect.h"
class EmissionEffect :
    public BaseEffect {

public:

    EmissionEffect();
    ~EmissionEffect() override;

    void Finalize() override;

    void Init(const std::string& directoryPath, const std::string& fileName, const uint32_t& particleNum) override;

    void Update(const Matrix4x4& viewMat, const Matrix4x4& projection) override;

    void Draw() override;

private:


};

