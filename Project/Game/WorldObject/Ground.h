#pragma once
#include "Engine/Assets/GameObject/BaseGameObject.h"

class Ground : public BaseGameObject {
public:

	Ground();
	~Ground();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:



};

