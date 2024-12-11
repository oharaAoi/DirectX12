#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 基本的な敵
/// </summary>
class NormalEnemy : public BaseGameObject {
public:

	NormalEnemy();
	~NormalEnemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:


};

