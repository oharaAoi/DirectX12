#include "GameResource.h"

GameResource::GameResource() {
}

GameResource::~GameResource() {
}

void GameResource::Load() {
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "cube.obj");

	// worldObject
	ModelManager::LoadModel("./Game/Resources/Skydome/", "skydome.obj");
	ModelManager::LoadModel("./Game/Resources/", "camera.obj");


}
