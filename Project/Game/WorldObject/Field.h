#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 大方となるマップ
/// </summary>
class Field : public BaseGameObject {
public:

	Field();
	~Field();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:



};