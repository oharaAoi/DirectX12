#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Collider/MeshCollider.h"
#include "Engine/Math/Easing.h"
#include "Engine/ParticleSystem/GpuEffect.h"
#include "Engine/Manager/CollisionManager.h"
#include "Engine/GameObject/TestObject.h"

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
	void ImGuiDraw();
#endif

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCamera_ = true;

	// ゲームオブジェクト ------------------------------------
	
	std::unique_ptr<TestObject> testObjA_;
	std::unique_ptr<TestObject> testObjB_;

	std::unique_ptr<MeshCollider> meshColliderA_;
	std::unique_ptr<MeshCollider> meshColliderB_;

	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<GpuEffect> gpuEffect_;

	int easeIndex_ = 0;

};

