#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Skydome :
	public BaseGameObject {
public:

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:


};

