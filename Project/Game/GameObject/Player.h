#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Player
	: public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

private:

};

