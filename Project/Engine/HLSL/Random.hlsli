
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

float3 RandomRange3d(float3 seed, float minVal, float maxVal) {
	return lerp(minVal, maxVal, rand3dTo3d(seed));
}

float RandomRange1d(float3 seed, float minVal, float maxVal) {
	return lerp(minVal, maxVal, rand3dTo1d(seed));
}

float3 RandomRange3d(float3 seed, float3 minVal, float3 maxVal) {
	return float3(
        lerp(minVal.x, maxVal.x, rand3dTo1d(float3(12.989, 78.233, 37.719))),
        lerp(minVal.y, maxVal.y, rand3dTo1d(float3(39.346, 11.135, 83.155))),
        lerp(minVal.z, maxVal.z, rand3dTo1d(float3(73.156, 52.235, 09.151)))
    );
}

class RandomGenerator {
	float3 seed;
	
	float3 Generated3d() {
		float result = rand3dTo1d(seed);
		seed = rand3dTo3d(seed);
		return seed;
	}
	float Generated1d() {
		float result = rand3dTo1d(seed);
		seed.x = result;
		return result;
	}
	
	float3 Generated3dRange(float minVal, float maxVal) {
		seed = RandomRange3d(seed, minVal, maxVal);
		return seed;
	}
	
	float Generated1dRange(float minVal, float maxVal) {
		float result = RandomRange1d(seed, minVal, maxVal);
		seed = rand3dTo3d(seed);
		return result;
	}
	
	float3 Generated3dRangeSize(float3 minVal, float3 maxVal) {
		seed = rand3dTo3d(seed);
		return lerp(minVal, maxVal, seed);
	}
};