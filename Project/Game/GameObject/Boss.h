#pragma once
#include <memory>
#include "Game/GameObject/BossBody.h"
#include "Game/GameObject/BossLeftHand.h"
#include "Game/GameObject/BossRightHand.h"

/// <summary>
/// Bossの大元となるクラス
/// </summary>
class Boss {
public:

	Boss();
	~Boss();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;


#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// 体
	std::unique_ptr<BossBody> boss_body_;
	// 左手
	std::unique_ptr<BossLeftHand> boss_leftHand_;
	// 右手
	std::unique_ptr<BossRightHand> boss_rightHand_;

};

