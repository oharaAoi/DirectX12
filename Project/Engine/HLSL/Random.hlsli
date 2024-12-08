
float rand3dTo1d(float3 value, float3 dotDir = float3(12.9898, 78.233, 37.719)) {
	float3 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return (random * 2.0) - 1.0;
}

float3 rand3dTo3d(float3 value) {
	return float3(
        rand3dTo1d(value, float3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float3(39.346, 11.135, 83.155)),
        rand3dTo1d(value, float3(73.156, 52.235, 09.151))
    );
}

class RandomGenerator {
	float3 seed;
	
	float3 Generated3d() {
		seed = rand3dTo3d(seed);
		return seed;
	}
	float Generated1d() {
		float result = rand3dTo1d(seed);
		seed.x = result;
		return result;
	}
};