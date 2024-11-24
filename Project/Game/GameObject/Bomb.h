#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Bomb : public BaseGameObject {
public:

	Bomb();
	~Bomb();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:


};

