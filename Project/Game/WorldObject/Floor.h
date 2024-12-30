#pragma once
#include "Engine/Assets/GameObject/BaseGameObject.h"

class Floor : public BaseGameObject {
public:

	Floor();
	~Floor();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:


};

