#pragma once
#include "Engine/Assets/GameObject/BaseGameObject.h"

class Skydome : public BaseGameObject {
public:

	Skydome();
	~Skydome();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:


};

