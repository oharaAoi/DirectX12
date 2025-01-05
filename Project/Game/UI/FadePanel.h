#pragma once
#include <memory>
#include "Engine/Assets/2d/Sprite.h"

class FadePanel {
public:

	FadePanel();
	~FadePanel();

	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// ブラックアウトをセットする
	/// </summary>
	void SetFadeOut(float timeLimit = 1.0f);

	/// <summary>
	/// ブラックアウトから開けさせる
	/// </summary>
	void SetFadeOutOpen(float timeLimit = 1.0f);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	/// <summary>
	/// fadeが終了したか
	/// </summary>
	/// <returns></returns>
	const bool GetIsFinished() const { return isFinished_; }

	/// <summary>
	/// Fadeを行っていないか
	/// </summary>
	/// <returns></returns>
	const bool GetDoNoting() const { return doNothing_; }

private:

	std::unique_ptr<Sprite> panel_;

	float panelAlpha_ = 0.0f;

	float time_ = 0.0f;
	float timeLimit_ = 1.0f;

	bool isBlackOut_;
	bool doNothing_;	// 何もしないフラグ
	bool isFinished_;	// 終了した
};


