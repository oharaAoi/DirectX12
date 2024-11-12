#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "Engine/Math/MyMatrix.h"
#include "Engine/Lib/Transform.h"
#include "Engine/GameObject/Model.h"

class BaseGameObject;

/// <summary>
/// アニメーションのNodeを階層構造を管理する機構
/// </summary>
class Skeleton {
public:

	struct Joint {
		QuaternionTransform transform;	// transform
		Matrix4x4 localMat;				// localMatrix
		Matrix4x4 skeltonSpaceMat;		// skeltonSpaceでの変換行列
		std::string name;				// 名前
		std::vector<int32_t> children;	// 子のJointのIndexのリスト
		int32_t index;					// 自身のIndex
		std::optional<int32_t> parent;	// 親のJointのIndex,いないとnll
	};

public:

	Skeleton();
	~Skeleton();

	void Init();
	void Update();

	void CreateSkeleton(const Model::Node& node);
	int32_t CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

public:

	const std::map<std::string, int32_t>& GetJointMap() const { return jointMap_; }

	std::vector<Joint>& GetJoints() { return joints_; }

	const size_t GetJointsSize() const { return joints_.size(); }

private:

	int32_t root_;	// RootJointのIndex
	std::map<std::string, int32_t> jointMap_;	// Joint名とIndexの辞書
	std::vector<Joint> joints_;	// 所属しているジョイント
};

