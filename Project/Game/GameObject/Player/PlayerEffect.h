#pragma once
#include <vector>
#include "Engine/GameObject/BaseGameObject.h"


struct PlayersGrain {
	std::unique_ptr<BaseGameObject> grain_;
	bool live_;
	float lifeTime_;
	float maxLifeTime_;
	Vector3 velocity_;
	Vector3 startVelo_;
	Vector3 endVelo_;
	Vector3 startSize_;
	Vector3 endSize_;
};

class PlayerEffect {
public:
	PlayerEffect();
	~PlayerEffect();

	void Init();
	void Update();
	void Draw();



	void EmitFragment(Vector3 pos, Vector2 dir);
	void EmitDown(Vector3 pos, Vector2 dir);


private:

	std::vector<std::unique_ptr<PlayersGrain>> fragument_;
	std::vector<std::unique_ptr<PlayersGrain>> stone_;


};
