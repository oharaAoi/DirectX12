#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <math.h>
#include <cmath>
#include "Engine/Math/Vector3.h"

struct KeysData {
	std::vector<aiVectorKey> vectorKeys;
	std::vector<aiQuatKey> quatKeys;
};

struct NodeAnimation {
	std::string nodeName;
	uint32_t numPositionKeys;
	KeysData positionData;
	uint32_t numRotationKeys;
	KeysData rotationData;
	uint32_t numScalingKeys;
	KeysData scalingData;
	aiAnimBehaviour postState;
	aiAnimBehaviour preState;

	// aiNodeAnim から NodeAnimation を初期化するコンストラクタ
	NodeAnimation(const aiNodeAnim* nodeAnim)
		: nodeName(nodeAnim->mNodeName.C_Str()),
		numPositionKeys(nodeAnim->mNumPositionKeys),
		numRotationKeys(nodeAnim->mNumRotationKeys),
		numScalingKeys(nodeAnim->mNumScalingKeys),
		postState(nodeAnim->mPostState),
		preState(nodeAnim->mPreState) {

		// PositionKeys をコピー
		positionData.vectorKeys.assign(nodeAnim->mPositionKeys, nodeAnim->mPositionKeys + nodeAnim->mNumPositionKeys);
		// RotationKeys をコピー
		rotationData.quatKeys.assign(nodeAnim->mRotationKeys, nodeAnim->mRotationKeys + nodeAnim->mNumRotationKeys);
		// ScalingKeys をコピー
		scalingData.vectorKeys.assign(nodeAnim->mScalingKeys, nodeAnim->mScalingKeys + nodeAnim->mNumScalingKeys);
	}
};

// 線形補間を行う補助関数
aiVector3D LinearInterpolation(const aiVector3D& start, const aiVector3D& end, float factor);

/// <summary>
/// nodeから取得した位置のアニメーションを行う
/// </summary>
/// <param name="nodeAnim">: aiNodeAnim</param>
/// <param name="time">: time</param>
/// <returns>補完した位置</returns>
aiVector3D InterpolationPosition(const NodeAnimation& nodeAnim, float time);

/// <summary>
/// </summary>
/// <param name="nodeAnim"></param>
/// <param name="time"></param>
/// <returns></returns>
aiVector3D InterpolationRotation(const NodeAnimation& nodeAnim, float time);