#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Scene/TestScene.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Manager/ModelManager.h"

/// <summary>
/// シーンを管理するクラス
/// </summary>
class SceneManager {
public:

	SceneManager();
	~SceneManager();

	void Finalize();
	void Init();
	void Run();

private:

	std::unique_ptr<BaseScene> scene_ = nullptr;

	EffectSystem* effectSystem_ = nullptr;

};

