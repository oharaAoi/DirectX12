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
	void SetBlackOut();

	/// <summary>
	/// ブラックアウトから開けさせる
	/// </summary>
	void SetBlackOutOpen();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> panel_;

	float time_ = 0.0f;
	float timeLimit_ = 1.0f;

};

