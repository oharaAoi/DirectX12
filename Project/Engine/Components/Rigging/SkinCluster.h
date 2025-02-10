#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include "Engine/Lib/Math/Matrix4x4.h"

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeight;
};

class SkinCluster {
public:

	SkinCluster();
	~SkinCluster();

	void Init();

	void AddData(std::map<std::string, JointWeightData> mapData);

	const std::map<std::string, JointWeightData>& GetSkinClustersData() { return skinClusterData_; }

private:

	std::map<std::string, JointWeightData> skinClusterData_;

};

