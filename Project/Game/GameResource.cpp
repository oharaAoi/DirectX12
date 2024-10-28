#include "GameResource.h"

GameResource::GameResource() {
}

GameResource::~GameResource() {
}

void GameResource::Load() {
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "cube.obj");
}
