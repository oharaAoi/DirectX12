#pragma once
#include <random>
#include "Engine/Math/Vector3.h"

inline std::random_device seedGenerator;
inline std::mt19937 randomEngine(seedGenerator());

float RandomFloat(const float& min, const float& max);
int RandomInt(int min, int max);

Vector3 RandomVector3(float min, float max);