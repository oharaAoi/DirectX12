#include "GameObjectWindow.h"
#include <sstream>

GameObjectWindow::GameObjectWindow() {}
GameObjectWindow::~GameObjectWindow() {}

void GameObjectWindow::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Objectの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameObjectWindow::AddFunction(std::function<void()> func, const std::string& label) {
	std::string uniqueLabel = label;
	int counter = 1;

	// ラベルがすでにリスト内に存在するかチェック
	auto isLabelDuplicate = [&](const std::string& labelToCheck) {
		for (const auto& pair : functionList) {
			if (pair.first == labelToCheck) {
				return true;
			}
		}
		return false;
		};

	// 重複があれば、末尾に番号を追加してユニークにする
	while (isLabelDuplicate(uniqueLabel)) {
		std::ostringstream oss;
		oss << label << counter++;
		uniqueLabel = oss.str();
	}

	// ユニークなラベルでリストに追加
	functionList.emplace_back(uniqueLabel, func);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集画面を表示する
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void GameObjectWindow::Edit() {
	ImGui::Begin("SceneObject");
	static int selectedEffectIndex = -1; // -1 means no selection
	int index = 0;
	for (auto it = functionList.begin(); it != functionList.end(); ++it, ++index) {
		std::string label = it->first;

		if (ImGui::Selectable(label.c_str(), selectedEffectIndex == index)) {
			selectedEffectIndex = index; // Update the selected index
		}
	}
	ImGui::End();

	ImGui::Begin("Object Setting");
	if (selectedEffectIndex >= 0 && selectedEffectIndex < functionList.size()) {
		auto it = functionList.begin();
		std::advance(it, selectedEffectIndex); // Move iterator to the selected index
		it->second();
	}
	ImGui::End();
}
#endif // _DEBUG
