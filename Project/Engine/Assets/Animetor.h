#pragma once
#include <memory>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Manager/AnimationManager.h"

class AnimetionClip;
class Skeleton;
class Skinning;

/// <summary>
/// SkeletonやSkinningの情報を持っているクラス
/// </summary>
class Animetor {
public:

	Animetor();
	~Animetor();

	void Finalize();
	void Update();

	void UpdateScript(float& animationTime, float transitionTime = 1.0f);

	void UpdateSkinning();

	/// <summary>
	/// animationのデータを読み込みつつ設定を行う
	/// </summary>
	/// <param name="directoryPath">: animationファイルがあるディレクトリパス</param>
	/// <param name="fileName">:animationファイルの名前</param>
	/// <param name="model">: 対応するモデル</param>
	/// <param name="isSkinning">: スキニングを行うか</param>
	/// <param name="isLoop">: animationのloopを行うか</param>
	/// <param name="isControlScript">: animationをスクリプトで制御するか</param>
	void LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model, bool isSkinning, bool isLoop, bool isControlScript);

	/// <summary>
	/// Animationを遷移させる
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="afterAnimation">: 遷移後アニメーション</param>
	void TransitionAnimation(const std::string& preAnimation, const std::string& afterAnimation);

	void TransitionAnimation(const std::string& afterAnimation);

#ifdef _DEBUG
	void Debug_Gui();

	std::string SelectAnimationName() { return animetionClip_->SelectAnimationName(); }

#endif

	Skinning* GetSkinning() { return skinning_.get(); }

	AnimetionClip* GetAnimationClip() { return animetionClip_.get(); }

	const Matrix4x4 GetAnimationMat() const { return animetionClip_->GetMatrix(); }

	const float GetAnimationDuration() const { return animetionClip_->GetAnimationDuration(); }

	// 今のanimationの名前を取得
	const std::string& GetAnimationName() const { return animetionClip_->GetAnimationName(); }

	// animationの時間を取得・設定
	const float GetAnimationTime() const { return animetionClip_->GetAnimationTime(); }	// 取得
	void SetAnimationTime(float time) { animetionClip_->SetAnimationTime(time); }		// 設定

	// animationの制御をスクリプトで行うかの取得・設定
	const bool GetIsControlScript() const { return isControlScript_; }			// 取得
	void SetIsControlScript(bool isControl) { isControlScript_ = isControl; }	// 設定

	// skinningを行うかを取得
	const bool GetIsSkinning() const { return isSkinning_; }

	// Animationの切り替えがあるかを取得
	const bool GetIsAnimationChange() const { return animetionClip_->GetIsChange(); }

	// モデルに入っているAnimationの名前を取得
	const std::vector<std::string>& GetAnimationNames() { return animetionClip_->GetAnimationNames(); }

private:

	AnimationManager* manager_ = nullptr;

	std::unique_ptr<AnimetionClip> animetionClip_ = nullptr;
	std::unique_ptr<Skeleton> skeleton_ = nullptr;
	std::unique_ptr<Skinning> skinning_ = nullptr;

	bool isSkinning_ = true;

	// -------------------------------------------------
	// ↓ Animationの遷移に関する変数
	// -------------------------------------------------
	bool isControlScript_ = false;

	float transitionTime_ = 0.0f;
};

