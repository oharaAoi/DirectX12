#pragma once
#include <memory>
#include "Engine/2d/Sprite.h"

/// <summary>
/// ブラックアウトさせる用のクラス
/// </summary>
class Panel {
public:

	Panel();
	~Panel();

	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// ブラックアウトをセットする
	/// </summary>
	void SetBlackOut(float timeLimit = 1.0f);

	/// <summary>
	/// ブラックアウトから開けさせる
	/// </summary>
	void SetBlackOutOpen(float timeLimit = 1.0f);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsFinished() const { return isFinished_; }

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

