#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Game/GameObject/DownEffect.h"
#include "Engine/Math/MyRandom.h"

class GameEffect {
public:

	GameEffect();
	~GameEffect();

	void Init();
	void Update();
	void Draw() const;

	void Pop(int popNum, const Vector3& popPos);

	static void NewPop(const Vector3& popPos);

private:

	std::list<std::unique_ptr<DownEffect>> downEffets_;

	static std::vector<Vector3> framePopPos_;

};

