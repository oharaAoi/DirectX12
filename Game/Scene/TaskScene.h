#pragma once
#include <map>
#include "Engine.h"
#include "Engine/Input/Input.h"
// lib
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/Mesh.h"
// gameObject
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Camera/Camera.h"
#include "Engine/GameObject/Sprite.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Audio/Audio.h"

enum CreateObjectKind {
	kTriangle,
	kSphere,
	kPlane,
	kTeapot,
	kBunny,
	kMultiMesh,
	kMultiMaterial,
	kSuzanne,
	kSprite
};

class TaskScene {

public:

	struct ObjectData {
		std::unique_ptr<BaseGameObject> gameObject;
		BaseGameObject* modelObject = nullptr;		// モデル用のポインタ
		WorldTransform worldTransform;
		std::string objectName;
	};

public:

	TaskScene();
	~TaskScene();

	void Init();

	void Update();

	void Draw();

	void AddObject();

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;
	
	// gameObject
	std::list<ObjectData> gameObjectList_;
	std::list<ObjectData> textureLessList_;
	std::list<std::unique_ptr<Sprite>> spriteList_;

	// model
	std::map<std::string, std::unique_ptr<Model>> modelMap_;

	Mesh::Vertices vertices_;
	Mesh::RectVetices rect_;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;

	int objectKind_ = 0; 
};

