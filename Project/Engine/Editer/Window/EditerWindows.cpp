#include "EditerWindows.h"

EditerWindows::~EditerWindows() {
}

EditerWindows* EditerWindows::GetInstance() {
	static EditerWindows instance;
	return &instance;
}

void EditerWindows::Init() {
	gameObjectWindow_ = std::make_unique<GameObjectWindow>();
	gameObjectWindow_->Init();
}

void EditerWindows::Update() {
#ifdef _DEBUG
	gameObjectWindow_->Edit();
#endif // _DEBUG
}

void EditerWindows::AddObjectWindow(std::function<void()> func, const std::string& label) {
	GetInstance()->GetObjectWindow()->AddFunction(func, label);
}
