#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Enemy : public BaseGameObject {
public:

	Enemy();
	~Enemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;
	
private:


};

