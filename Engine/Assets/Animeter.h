#pragma once
#include <vector>
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Assets/Skeleton.h"

/// <summary>
/// Animationに関するクラス
/// </summary>
class Animeter {
public:

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
	};

public:

	Animeter();
	~Animeter();

	void Init();
	void Update();

	void LoadAnimation(const std::string directoryPath, const std::string& animationFile);

	void ApplyAnimation(Skeleton& skelton);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time);
	Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, const float& time);

public:

	const Matrix4x4 GetMatrix() const { return animationMat_; }

private:

	Animation animation_;

	float animationTime_ = 0.0f;

	Matrix4x4 animationMat_;

};


