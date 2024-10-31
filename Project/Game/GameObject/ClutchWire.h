#pragma once
#include "Engine/GameObject/BaseGameObject.h"


class ClutchWire : public BaseGameObject {
public:
	ClutchWire();
	~ClutchWire();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;


#ifdef _DEBUG
	void Debug_Gui();
#endif

private:


};
