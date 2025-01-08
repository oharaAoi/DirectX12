#pragma once
#include "Engine.h"
#include "Engine/Utilities/Loader.h"
#include "Game/Scene/SceneFactory.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Scene/TestScene.h"
#include "Game/Scene/GameScene.h"
#include "Engine/System/ParticleSystem/EffectSystem.h"
#include "Engine/System/Manager/ModelManager.h"
#include "Engine/System/Input/Input.h"
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
	void Update();
	void Draw();

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	void SetChange(const SceneType& type);

private:

	std::unique_ptr<SceneFactory> sceneFactory_;

	std::unique_ptr<BaseScene> scene_ = nullptr;
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	EffectSystem* effectSystem_ = nullptr;

	GameResources resources_;

#ifdef _DEBUG
	int selectSceneNum_ = 0;
	bool isSceneChange_ = 0;

	SceneType changeScene_;
#endif // _DEBUG

};

