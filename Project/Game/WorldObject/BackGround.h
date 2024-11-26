#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class BackGround : public BaseGameObject {
public:

	BackGround();
	~BackGround();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:


};

