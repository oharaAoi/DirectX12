#include "RailPointEditer.h"
#include "Render.h"

RailPointEditer::RailPointEditer() {}
RailPointEditer::~RailPointEditer() {}

void RailPointEditer::Init() {
	//std::vector<Vector3> points = {
	//{ 0.0f, 0.0f, 0.0f },    
	//{ 2.0f, 0.0f, 1.0f },    
	//{ 5.0f, 0.0f, 1.5f },   
	//{ 7.0f, 2.0f, 2.0f },   
	//{ 6.0f, 3.0f, 3.0f },   
	//{ 3.0f, 5.0f, 4.0f },   
	//{ 0.0f, 6.0f, 5.0f },   
	//{ -2.0f, 5.0f, 6.0f },  
	//{ -3.0f, 3.0f, 7.0f },  
	//{ -4.0f, 0.0f, 8.0f },  
	//{ 0.0f, -1.0f, 9.0f },  
	//{ 3.0f, -1.5f, 10.0f }, 
	//{ 5.0f, -2.0f, 11.0f }, 
	//{ 7.0f, 0.0f, 12.0f },  
	//{ 10.0f, 1.0f, 13.0f }, 
	//{ 12.0f, 1.5f, 14.0f }, 
	//{ 13.0f, 0.5f, 15.0f }, 
	//{ 15.0f, 0.0f, 16.0f }, 
	//{ 17.0f, -1.0f, 17.0f },
	//{ 19.0f, -2.0f, 18.0f },
	//{ 20.0f, -3.0f, 19.0f },
	//{ 18.0f, -2.0f, 20.0f },
	//{ 15.0f, 0.0f, 21.0f }, 
	//{ 10.0f, 1.0f, 22.0f }, 
	//{ 5.0f, 0.0f, 23.0f },  
	//{ 0.0f, 0.0f, 24.0f },  
	//{ -5.0f, 0.0f, 25.0f }, 
	//{ -10.0f, 0.5f, 26.0f },
	//{ -12.0f, 2.0f, 27.0f },
	//{ -15.0f, 3.0f, 28.0f } 
	//};

	std::vector<Vector3> points;
	for (uint32_t oi = 0; oi < 30; ++oi) {
		Vector3 pos = { 0.0f, 0.0f, 2.0f + oi };
		points.push_back(pos);
	}

	for (uint32_t oi = 0; oi < points.size(); ++oi) {
		RailData data(points[oi], 0.0f);
		railPoints.push_back(data);
	}
}

void RailPointEditer::Update() {
	if (railPoints.size() < 4) {
		return;
	}

	railIndexPoints_.clear();
	std::vector<Vector3> copyPos;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		copyPos.push_back(railPoints[oi].centerPosiont);
	}

	const size_t segmentCount = 100;
	// 点からSpline曲線を引く
	for (uint32_t oi = 0; oi < segmentCount; ++oi) {
		float t = (1.0f / static_cast<float>(segmentCount)) * oi;
		Vector3 pos = CatmullRomPosition(copyPos, t);
		railIndexPoints_.push_back(pos);
	}
}

void RailPointEditer::Draw(const Matrix4x4& vpMat) const {
	// 線を描画する
	for (size_t oi = 0; oi < railIndexPoints_.size() - 2; ++oi) {
		Render::DrawLine(railIndexPoints_[oi], railIndexPoints_[oi + 1], {0.0f, 0.0f, 0.0f, 1.0f}, vpMat);
	}
}

void RailPointEditer::DrawObject() {

}

void RailPointEditer::Save() {

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

	/*for (uint32_t oi = 0; oi < railPoints.size(); oi += 10) {
		std::string startName = std::to_string(oi);
		std::string endName = std::to_string(oi + 10);
		std::string name = startName + endName;
		if (ImGui::TreeNode(name.c_str())) {
			
			ImGui::End();
		}
	}*/
}

void RailPointEditer::Debug_Gui() {
	ImGui::Begin("railPoint");
	EditRail();
	ImGui::End();
}
std::vector<Vector3> RailPointEditer::GetRailPoints() {
	return railIndexPoints_;
}
std::vector<Vector3> RailPointEditer::GetRailBasePoints() {
	std::vector<Vector3> result;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		result.push_back(railPoints[oi].centerPosiont);
	}
	return result;
}
#endif