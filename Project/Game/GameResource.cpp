#include "GameResource.h"

GameResource::GameResource() {
}

GameResource::~GameResource() {
}

void GameResource::Load() {
	ModelManager::LoadModel("./Engine/Resources/Animation/", "amimationCharacter.gltf");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "cube.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "axis.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "plane.obj");
	//ModelManager::LoadModel("./Engine/Resources/Develop/", "test.fbx");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "simpleSkin.gltf");
	//ModelManager::LoadModel("./Engine/Resources/Animation/", "amimationCharacter.fbx");
	

}
