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

class AnimationManager;

/// <summary>
/// Animationに関するクラス
/// </summary>
class AnimetionClip {
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

	AnimetionClip();
	~AnimetionClip();

	void Init();
	void Update();

	void LoadAnimation(const std::string directoryPath, const std::string& animationFile, const std::string& rootName, bool isSkinning);
	void LoadGetAnimation(const std::string& animationFile, bool isSkinning);

	void ApplyAnimation(Skeleton* skelton);

	void LerpApplyAnimation(Skeleton* skelton);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time);
	Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, const float& time);

	void SetLerpAnimation(const std::string& preAnimation, const std::string& lerpAnimation);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	float GetAnimationTime() const { return animationTime_; }
	void SetAnimationTime(float time) { animationTime_ = time; }

	const Matrix4x4 GetMatrix() const { return animationMat_; }

	const bool GetIsAnimationFinish() const { return isAnimationFinish_; }

	const bool GetIsChange() const { return isAnimationChange_; }

private:

	AnimationManager* manager_;

	// 実際にアウル動き
	Animation animation_;
	// アニメーションの時間
	float animationTime_ = 0.0f;
	bool isAnimationFinish_;

	Matrix4x4 animationMat_;

	std::string rootName_;

	// アニメーションの入っているファイルの名前
	std::string animationFileName_;
	// アニメーションの名前の配列
	std::vector<std::string> animationNames_;
	// skinningを行うか
	bool isSkinnig_;

	int selectedAnimationIndex = 0;

	// -------------------------------------------------
	// ↓ アニメーションの遷移に関する変数
	// -------------------------------------------------
	bool isAnimationChange_;			// アニメーションの遷移を行うか
	float blendFactor_;					// 補完の線形代数
	Animation lerpAnimetion_[2];		// 補完させるアニメーション
	float lerpAnimationTime_[2];		// アニメーションさせるkeyTime
	int lerpAnimationNamesIndex_[2];	// 
};


