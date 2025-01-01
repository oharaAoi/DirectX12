#include "EditerWindows.h"

EditerWindows::~EditerWindows() {}
EditerWindows* EditerWindows::GetInstance() {
	static EditerWindows instance;
	return &instance;
}

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EditerWindows::Init() {
	gameObjectWindow_ = std::make_unique<GameObjectWindow>();
	gameObjectWindow_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EditerWindows::Update() {
	gameObjectWindow_->Edit();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　開始時処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EditerWindows::Begine() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(kWindowWidth_, kWindowHeight_));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("BaseWindow", nullptr, window_flags)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

	}
	ImGuiID dockspace_id = ImGui::GetID("BaseDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了時処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EditerWindows::End() {
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Objectの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void EditerWindows::AddObjectWindow(std::function<void()> func, const std::string& label) {
	GetInstance()->GetObjectWindow()->AddFunction(func, label);
}
#endif // _DEBUG