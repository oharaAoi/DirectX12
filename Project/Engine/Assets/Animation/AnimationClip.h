#pragma once
#include <vector>
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Assets/Rigging/Skeleton.h"

class AnimationManager;

/// <summary>
/// Animationに関するクラス
/// </summary>
class AnimationClip {
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
		std::string animationName;
	};

public:

	AnimationClip();
	~AnimationClip();

	void Init();
	void Update();

	void LoadAnimation(const std::string directoryPath, const std::string& animationFile, const std::string& rootName, bool isSkinning);
	void LoadGetAnimation(const std::string& animationFile, bool isSkinning);

	void ApplyAnimation(Skeleton* skelton);

	/// <summary>
	/// animetionを遷移させる
	/// </summary>
	/// <param name="skelton">: skelton</param>
	void LerpApplyAnimation(Skeleton* skelton);

	/// <summary>
	/// animetionを時間を指定して遷移させる
	/// </summary>
	/// <param name="skelton">: skelton</param>
	/// <param name="time">: 遷移の時間</param>
	void AnimationTransition(Skeleton* skelton, float time);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time);
	Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, const float& time);

	/// <summary>
	/// animationを遷移させる設定をする
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>

	/// <summary>
	/// animationを遷移させる設定をする
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	/// <param name="blendSpeed">: 遷移する速さ</param>
	void SetLerpAnimation(const std::string& preAnimation, const std::string& lerpAnimation, float blendSpeed);

	/// <summary>
	/// animationを遷移させる設定をする
	/// </summary>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	void SetLerpAnimation(const std::string& lerpAnimation, float blendSpeed);

	void SetAnimation(const std::string& animationName, float blendSpeed);

	/// <summary>
	/// Animationの予約
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	/// <param name="startTransitionRaito">: 前のAnimationがどのくらいの割合の時に開始するか</param>
	void SetAnimationReservation(const std::string& preAnimation, const std::string& lerpAnimation, float blendSpeed, float startTransitionRaito);

#ifdef _DEBUG
	void Debug_Gui();

	std::string SelectAnimationName();
#endif

	// animationの時間を取得・設定
	float GetAnimationTime() const { return animationTime_; }		// 取得
	void SetAnimationTime(const float& time) { animationTime_ = time; }	// 設定

	// animationの最大フレーム数を取得
	const float GetAnimationDuration() const { return animation_.duration; }

	// 今のAnimationの名前を取得する
	const std::string& GetAnimationName() const { return animation_.animationName; }

	// animationの行列を取得
	const Matrix4x4 GetMatrix() const { return animationMat_; }

	// animationが終了しているかを取得
	const bool GetIsAnimationFinish() const { return isAnimationFinish_; }

	// animatinoの切り替えがあるかを取得
	const bool GetIsChange() const { return isAnimationChange_; }

	// animationをloopさせるかを取得・設定
	const bool GetIsLoop() const { return isLoop_; }	// 取得
	void SetIsLoop(bool isLoop) { isLoop_ = isLoop; }	// 設定

	// 予約があるかの取得
	const bool GetIsReservation() const { return isReservation_; }

	// モデルに入っているAnimationの名前を取得
	const std::vector<std::string>& GetAnimationNames() { return animationNames_; }

	// Animationの速度
	void SetAnimationSpeed(float speed) { animationSpeed_ = speed; }

private:

	AnimationManager* manager_;

	// 実際の動き
	Animation animation_;
	std::string nowAnimationName_;
	// アニメーションの時間
	float animationTime_ = 0.0f;
	// アニメーションの速度
	float animationSpeed_ = 1.0f;

	Matrix4x4 animationMat_ = Matrix4x4::MakeUnit();

	std::string rootName_;
	// アニメーションの入っているファイルの名前
	std::string animationFileName_;
	// アニメーションの名前の配列
	std::vector<std::string> animationNames_;

	// skinningを行うか
	bool isSkinnig_ = true;
	// Animation終了フラグ
	bool isAnimationFinish_ = false;
	// アニメーションをループさせるか
	bool isLoop_ = false;

	bool isStop_ = false;

	int selectedAnimationIndex = 0;

	// -------------------------------------------------
	// ↓ アニメーションの遷移に関する変数
	// -------------------------------------------------
	bool isAnimationChange_;			// アニメーションの遷移を行うか
	float blendFactor_;					// 補完の線形代数
	float blendSpeed_;
	Animation lerpAnimetion_[2];		// 補完させるアニメーション
	float lerpAnimationTime_[2];		// アニメーションさせるkeyTime
	int lerpAnimationNamesIndex_[2];	// 

	bool isReservation_;				// Animationの予約をする
	float startTransitionRaito_;		// 遷移を開始する割合
};


