#pragma once
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/Components/Collider/MeshCollider.h"
#include "Engine/System/Manager/CollisionManager.h"
#include "Engine/Components/GameObject/TestObject.h"
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
	std::unique_ptr<Camera2d> camera2d_ = nullptr;
	std::unique_ptr<Camera3d> camera3d_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCamera_ = true;

	// worldObject ------------------------------------
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Floor> floor_ = nullptr;

	// gameObject ------------------------------------
	std::unique_ptr<TestObject> testObjA_;
	
	std::unique_ptr<MeshCollider> meshColliderA_;
	std::unique_ptr<MeshCollider> meshColliderB_;

	std::unique_ptr<CollisionManager> collisionManager_;

	float raito_ = 0.9f;
};

