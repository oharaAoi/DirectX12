#pragma once
#include <random>

inline std::random_device seedGenerator;
inline std::mt19937 randomEngine(seedGenerator());

float RandomFloat(const float& min, const float& max);
int RandomInt(int min, int max);