#pragma once
#include <map>
#include <vector>
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"

template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

/// <summary>
/// Nodeごとのアニメーション
/// </summary>
struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation {
	float duration;		// アニメーション全体の尺
	// NodeAnimationの集合。Node名で引けるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
	std::string animationName;
};