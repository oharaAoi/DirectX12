#pragma once
#include <memory>
#include "Engine/Editer/Window/GameObjectWindow.h"

class EditerWindows {
public:

	EditerWindows() = default;
	~EditerWindows();

	EditerWindows(const EditerWindows&) = delete;
	const EditerWindows& operator=(const EditerWindows&) = delete;

	static EditerWindows* GetInstance();

#ifdef _DEBUG
	void Init();

	void Update();

	/// <summary>
	/// 開始処理
	/// </summary>
	void Begine();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

public:

	static void AddObjectWindow(std::function<void()> func, const std::string& label);

private:

	GameObjectWindow* GetObjectWindow() { return gameObjectWindow_.get(); }

private:

	std::unique_ptr<GameObjectWindow> gameObjectWindow_;

#endif // _DEBUG
};

