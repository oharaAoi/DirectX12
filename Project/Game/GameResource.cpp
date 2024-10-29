#include "GameResource.h"

GameResource::GameResource() {
}

GameResource::~GameResource() {
}

void GameResource::Load() {
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "cube.obj");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");

	// worldObject
	ModelManager::LoadModel("./Game/Resources/Model/Skydome/", "skydome.obj");
	ModelManager::LoadModel("./Game/Resources/", "camera.obj");
	ModelManager::LoadModel("./Game/Resources/", "rail.obj");

	// gameObject
	ModelManager::LoadModel("./Game/Resources/Model/Star/", "star.obj");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "reticle.png");

}
