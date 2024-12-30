#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/System/ParticleSystem/EffectSystem.h"
#include "Engine/Assets/Collider/MeshCollider.h"
#include "Engine/Math/Easing.h"
#include "Engine/System/Manager/CollisionManager.h"
#include "Engine/Assets/GameObject/TestObject.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/WorldObject/Floor.h"

class TestScene 
: public BaseScene {
public:


public:

	TestScene();
	~TestScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	// camera ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCamera_ = true;

	// worldObject ------------------------------------
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Floor> floor_ = nullptr;

	// gameObject ------------------------------------
	
	std::unique_ptr<TestObject> testObjA_;
	std::unique_ptr<TestObject> testObjB_;

	std::unique_ptr<MeshCollider> meshColliderA_;
	std::unique_ptr<MeshCollider> meshColliderB_;

	std::unique_ptr<CollisionManager> collisionManager_;
};

