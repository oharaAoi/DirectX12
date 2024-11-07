#pragma once
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Math/MyMath.h"

class Energy {
public:

	Energy();
	~Energy();

	void Init();
	void Update(float energy);
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> outside_;
	std::unique_ptr<Sprite> energy_;

	Vector2 centerPos_;

};

