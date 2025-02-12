#include "GameObjectWindow.h"
#include <sstream>

GameObjectWindow::GameObjectWindow() {}
GameObjectWindow::~GameObjectWindow() {}

void GameObjectWindow::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Objectの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameObjectWindow::AddFunction(AttributeGui* attribute, const std::string& label) {
	std::string uniqueLabel = label;
	int counter = 1;

	// ラベルがすでにリスト内に存在するかチェック
	auto isLabelDuplicate = [&](const std::string& labelToCheck) {
		for (const auto& pair : attributeArray_) {
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
	attributeArray_.emplace_back(uniqueLabel, attribute);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集画面を表示する
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void GameObjectWindow::Edit() {
	// -------------------------------------------------
	// ↓ Itemの選択
	// -------------------------------------------------
	ImGui::Begin("Scene Object");
	static AttributeGui* selectAttribute = nullptr;
	int index = 0;
	for (auto it = attributeArray_.begin(); it != attributeArray_.end(); ++it, ++index) {
		std::string label = it->first;
		const AttributeGui* ptr = it->second;

		if (ptr->HasChild()) {
			if (ImGui::TreeNode(label.c_str())) {
				for (auto child : ptr->GetChildren()) {
					if (ImGui::TreeNode(child->GetName().c_str())) {
						selectAttribute = child;
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		} else {
			if (ImGui::Selectable(label.c_str(), selectAttribute == ptr)) {
				selectAttribute = it->second;
			}
		}
	}
	ImGui::End();

	// -------------------------------------------------
	// ↓ ItemごとのImGuiを編集する
	// -------------------------------------------------
	ImGui::Begin("Object Setting");
	if (selectAttribute != nullptr) {
		selectAttribute->Debug_Gui();
	}
	//if (selectedEffectIndex >= 0 && selectedEffectIndex < attributeArray_.size()) {
	//	auto it = attributeArray_.begin();
	//	std::advance(it, selectedEffectIndex); // Move iterator to the selected index
	//	it->second->Debug_Gui();
	//}
	ImGui::End();
}
#endif // _DEBUG
