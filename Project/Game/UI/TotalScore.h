#pragma once
#include <vector>
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Math/MyMath.h"

class TotalScore {
public:

	TotalScore();
	~TotalScore();

	void Init();
	void Update(uint32_t playerScore);
	void Draw() const;

	void AddArray();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> background_;

	std::vector<std::unique_ptr<Sprite>> score_;
	uint32_t playerScore_;

	Vector2 numberSpriteSize_;
	Vector2 originPos_;

};