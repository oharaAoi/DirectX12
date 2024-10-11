#include "RailPointEditer.h"

RailPointEditer::RailPointEditer() {}
RailPointEditer::~RailPointEditer() {}

void RailPointEditer::Init() {
}

void RailPointEditer::Update() {
#ifdef _DEBUG
	Debug_Gui();
#endif
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void RailPointEditer::EditRail() {
	// -------------------------------------------------
	// ↓ 新しい点を追加する
	// -------------------------------------------------
	ImGui::DragFloat3("newPoint", &newPoint_.x, 0.1f);
	ImGui::DragFloat("newTwist", &newTwist_, 0.1f);
	if (ImGui::Button("Add")) {
		railPoints.emplace_back(RailData(newPoint_, newTwist_));
	}

	// -------------------------------------------------
	// ↓ 今ある点を編集する
	// -------------------------------------------------
	if (ImGui::InputText("pointIndex", selectPointIndex_, sizeof(selectPointIndex_))) {
		selectIndex_ = selectPointIndex_;
	}

	if (selectIndex_ != "") {
		// 配列の最大数を表示
		if (!railPoints.empty()) {
			ImGui::Text("maxIndex %d", static_cast<int>(railPoints.size()));
		}

		// 配列の中身を表示
		if (std::stoi(selectIndex_) < railPoints.size()) {
			ImGui::DragFloat3("point", &railPoints[std::stoi(selectIndex_)].centerPosiont.x, 0.1f);
			ImGui::DragFloat("twist", &railPoints[std::stoi(selectIndex_)].twist, 0.1f);
		}
	}
}

void RailPointEditer::Debug_Gui() {
	ImGui::Begin("railPoint");
	EditRail();
	ImGui::End();
}
#endif