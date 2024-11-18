#include "GameResource.h"

GameResource::GameResource() {
}

GameResource::~GameResource() {
}

void GameResource::Load() {
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "cube.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "axis.obj");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");

	// worldObject
	ModelManager::LoadModel("./Game/Resources/Model/Skydome/", "skydome.obj");
	ModelManager::LoadModel("./Game/Resources/", "camera.obj");
	ModelManager::LoadModel("./Game/Resources/", "rail.obj");

	// gameObject
	ModelManager::LoadModel("./Game/Resources/Model/Star/", "star.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Star/", "effectStar.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Razer/", "razer.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Cart/", "cart.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Board/", "board.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "ground.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Balloon/", "balloon.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Jet/", "jet.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Boss/", "boss.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Gate/", "gate.obj");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "reticle.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "rail_plane.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "scoreBackground.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "razer_outside.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "energy.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "title.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "panel.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "clear.png");

	AudioManager::LoadAudio("./Game/Resources/Audio/", "hit.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "shot.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "guutara.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "push.mp3");
	AudioManager::LoadAudio("./Engine/Resources/Develop/Audio/", "fanfare.wav");
}
