#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// bossの胴体となるクラス
/// </summary>
class BossBody : public BaseGameObject {
public:

	BossBody();
	~BossBody();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:



};

