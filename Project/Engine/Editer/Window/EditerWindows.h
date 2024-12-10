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

	void Init();

	void Update();

public:

	static void AddObjectWindow(std::function<void()> func, const std::string& label);

private:

	GameObjectWindow* GetObjectWindow() { return gameObjectWindow_.get(); }

private:

	std::unique_ptr<GameObjectWindow> gameObjectWindow_;

};

