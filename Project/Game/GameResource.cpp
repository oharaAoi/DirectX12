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
	//ModelManager::LoadModel("./Engine/Resources/Develop/", "test.fbx");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "simpleSkin.gltf");
	//ModelManager::LoadModel("./Engine/Resources/Animation/", "amimationCharacter.fbx");
	

	// worldObject
	ModelManager::LoadModel("./Game/Resources/Model/Skydome/", "skydome.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Field/", "kair_field.obj");
	ModelManager::LoadModel("./Game/Resources/", "camera.obj");
	ModelManager::LoadModel("./Game/Resources/", "rail.obj");

	// gameObject
	ModelManager::LoadModel("./Game/Resources/Model/Star/", "star.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Body/", "boss_body.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Left_Hand/", "Left_Hand.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Right_Hand/", "Right_Hand.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Right_Hand/", "Right_Hand.gltf");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "reticle.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "rail_plane.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "scoreBackground.png");

}
