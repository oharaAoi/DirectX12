#pragma once
#include "Engine.h"
#include "Engine/Utilities/Loader.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Scene/TestScene.h"
#include "Game/Scene/GameScene.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/Input/Input.h"
#include "Game/GameResources.h"

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

	void SetChange();
	void GetScene();

private:

	GameTimer gameTimer_ = GameTimer(60);

	std::unique_ptr<BaseScene> scene_ = nullptr;

	EffectSystem* effectSystem_ = nullptr;

	GameResources resources_;

#ifdef _DEBUG
	int selectSceneNum_ = 0;
	bool isSceneChange_ = 0;
#endif // _DEBUG

};

