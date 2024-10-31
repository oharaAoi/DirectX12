#pragma once
#include <memory>
#include "Game/GameObject/BossBody.h"


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

	std::unique_ptr<BossBody> boss_body_;

};

