#pragma once
#include <memory>
#include "Engine/Core/AoiFramework.h"
#include "Engine/System/Manager/SceneManager.h"

/// <summary>
/// ゲーム全体
/// </summary>
class GameCore : public AoiFramework {
public:

	GameCore();
	~GameCore();

	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<SceneManager> sceneManger_;

};

