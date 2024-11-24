#pragma once
#include "Engine.h"
#include "Game/Scene/SceneFactory.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/ParticleSystem/EffectSystem.h"
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

	void PostFrame();

	void SetChange(const SceneType& type);

	const bool GetIsChange() const { return isChange_; }
	void SetIsChange(bool isChange) { isChange_ = isChange; }

	const SceneType GetNowSceneType() const { return nowSceneType_; }

#ifdef _DEBUG
	void Show_Gui();
#endif // _DEBUG

private:

	std::unique_ptr<SceneFactory> sceneFactory_;

	GameTimer gameTimer_ = GameTimer(60);

	std::unique_ptr<BaseScene> scene_ = nullptr;
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	EffectSystem* effectSystem_ = nullptr;

	GameResources resources_;

	SceneType nowSceneType_;
	std::string nowSceneTypeString_;

	bool isChange_;

#ifdef _DEBUG
	int selectSceneNum_ = 0;
	bool isSceneChange_ = 0;
#endif // _DEBUG

};

