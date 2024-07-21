#pragma once
#include "Engine.h"
#include "Input.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "BaseGameObject.h"
#include "Camera.h"

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

	int objectKind_ = 0; 
};

