#include "MyRandom.h"

float RandomFloat(const float& min, const float& max) {
    std::uniform_real_distribution<float> distribution(min, max);
    float result = distribution(randomEngine);
    return result;
}

int RandomInt(int min, int max){
    std::uniform_int_distribution<int> distribution(min, max);
    int result = distribution(randomEngine);
    return result;
}

Vector3 RandomVector3(float min, float max) {
    return Vector3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}
