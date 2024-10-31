#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 大方となるマップ
/// </summary>
class Field : public BaseGameObject {
public:

	Field();
	~Field();

	void Init();
	void Update();
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:



};