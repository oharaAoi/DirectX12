#include "RailPointEditer.h"
#include "Render.h"

RailPointEditer::RailPointEditer() {}
RailPointEditer::~RailPointEditer() {}

void RailPointEditer::Init() {
	/*std::vector<Vector3> points = {
	{ 0.0f, 0.0f, 0.0f },
	{ 2.0f, 0.0f, 1.0f },
	{ 5.0f, 0.0f, 1.5f },
	{ 7.0f, 2.0f, 2.0f },
	{ 6.0f, 3.0f, 3.0f },
	{ 3.0f, 5.0f, 4.0f },
	{ 0.0f, 6.0f, 5.0f },
	{ -2.0f, 5.0f, 6.0f },
	{ -3.0f, 3.0f, 7.0f },
	{ -4.0f, 0.0f, 8.0f },
	{ 0.0f, -1.0f, 9.0f },
	{ 3.0f, -1.5f, 10.0f },
	{ 5.0f, -2.0f, 11.0f },
	{ 7.0f, 0.0f, 12.0f },
	{ 10.0f, 1.0f, 13.0f },
	{ 12.0f, 1.5f, 14.0f },
	{ 13.0f, 0.5f, 15.0f },
	{ 15.0f, 0.0f, 16.0f },
	{ 17.0f, -1.0f, 17.0f },
	{ 19.0f, -2.0f, 18.0f },
	{ 20.0f, -3.0f, 19.0f },
	{ 18.0f, -2.0f, 20.0f },
	{ 15.0f, 0.0f, 21.0f },
	{ 10.0f, 1.0f, 22.0f },
	{ 5.0f, 0.0f, 23.0f },
	{ 0.0f, 0.0f, 24.0f },
	{ -5.0f, 0.0f, 25.0f },
	{ -10.0f, 0.5f, 26.0f },
	{ -12.0f, 2.0f, 27.0f },
	{ -15.0f, 3.0f, 28.0f }
	};*/

	Vector3 start(0, 0, 0);
	Vector3 direction(1, 0, 0); // x軸方向に進む場合

	// 10個の点を配置
	//std::vector<Vector3> points = createVector3Points(start, direction, 30);

	std::vector<Vector3> points;
	for (uint32_t oi = 0; oi < 5; ++oi) {
		Vector3 pos = { 0.0f, 0.0f, 2.0f * oi };
		points.push_back(pos);
	}

	//for (uint32_t oi = 0; oi < points.size(); ++oi) {
	//	RailPointData data(points[oi], Quaternion());
	//	railPoints.push_back(data);
	//}

	newRail_ = std::make_unique<Rail>();
	newRail_->Init();

	std::filesystem::path dire(directoryPath_);
	if (!std::filesystem::exists(directoryPath_)) {
		std::filesystem::create_directories(directoryPath_);
	}

	railPoints.clear();
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath_)) {
		std::string fileName = entry.path().stem().string();
		Load();
	}
}

void RailPointEditer::Update() {
	if (railPoints.size() < 4) {
		return;
	}

	railIndexPoints_.clear();
	railIndexPointsRotateZ_.clear();
	std::vector<Vector3> copyPos;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		copyPos.push_back(railPoints[oi].pos);
	}

	std::vector<Quaternion> copyRotate;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		copyRotate.push_back(railPoints[oi].rotate.Normalize());
	}

	const size_t segmentCount = 100;
	// 点からSpline曲線を引く
	for (uint32_t oi = 0; oi < segmentCount; ++oi) {
		float t = (1.0f / static_cast<float>(segmentCount)) * oi;
		Vector3 pos = CatmullRomPosition(copyPos, t);
		railIndexPoints_.push_back(pos);
	}

	// 点からSpline曲線を引く
	for (uint32_t oi = 0; oi < segmentCount; ++oi) {
		float t = (1.0f / static_cast<float>(segmentCount)) * oi;
		Quaternion rotate = Quaternion::CatmullRomRotate(copyRotate, t);
		railIndexPointsRotateZ_.push_back(Quaternion::GetYawFromQuaternion(rotate));
	}
}

void RailPointEditer::Draw(const Matrix4x4& vpMat) const {
	// 線を描画する
	for (size_t oi = 0; oi < railIndexPoints_.size() - 2; ++oi) {
		Render::DrawLine(railIndexPoints_[oi], railIndexPoints_[oi + 1], { 1.0f, 1.0f, 1.0f, 1.0f }, vpMat);
	}
}

void RailPointEditer::DrawObject() {
	newRail_->Draw();
}

std::vector<Vector3> RailPointEditer::createVector3Points(const Vector3& start, const Vector3& direction, int count) {
	std::vector<Vector3> points;
	points.push_back(start);

	Vector3 current = start;
	for (int i = 1; i < count; ++i) {
		// 新しい点を作成 (directionは正規化されていると仮定して2を掛ける)
		Vector3 next(current.x + direction.x * 2, current.y + direction.y * 2, current.z + direction.z * 2);
		points.push_back(next);
		current = next;  // currentを更新
	}

	return points;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailPointEditer::Load() {
	std::string filePath = directoryPath_ + railFile_ + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + railFile_ + ".json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	std::map<int, json> sortedRailPoints;
	// キーを番号順に格納
	for (auto& item : root["railData"].items()) {
		// キー名から番号を取り出す
		std::string key = item.key();
		size_t pos = key.find_last_of('_');
		if (pos != std::string::npos) {
			int index = std::stoi(key.substr(pos + 1));  // 番号を抽出
			sortedRailPoints[index] = item.value();  // 番号順に格納
		}
	}

	for (const auto& [index, value] : sortedRailPoints) {
		Vector3 pos = { value["pos"][0], value["pos"][1], value["pos"][2] };
		Quaternion rotate(value["rotate"][0], value["rotate"][1], value["rotate"][2], value["rotate"][3]);

		railPoints.emplace_back(RailPointData(pos, rotate));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailPointEditer::Save() {
	json root;
	for (uint32_t index = 0; index < railPoints.size(); ++index) {
		std::string id = "RailPointData_" + std::to_string(index);

		json pos = json::array({ railPoints[index].pos.x,railPoints[index].pos.y, railPoints[index].pos.z });
		json saveVelocity = json::array({ railPoints[index].rotate.x,railPoints[index].rotate.y, railPoints[index].rotate.z, railPoints[index].rotate.w });

		root[railFile_][id] = {
			{"pos", pos},
			{"rotate", saveVelocity},
		};
	}

	// 実際にファイルに保存する
	std::string filePath = directoryPath_ + railFile_.c_str() + ".json";

	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (!ofs.fail()) {
		assert("not open File");
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}

const Vector3 RailPointEditer::GetRailPos(size_t index) const {
	return railPoints[index].pos;
}

const Quaternion RailPointEditer::GetRailRotate(size_t index) const {
	return railPoints[index].rotate;
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void RailPointEditer::EditRail() {
	// -------------------------------------------------
	// ↓ 新しい点を追加する
	// -------------------------------------------------
	ImGui::DragFloat3("newPoint", &newPoint_.x, 0.1f);
	Quaternion::Debug_Gui(newRotate_, "new_rotate");
	//ImGui::DragFloat("newTwist", &newTwist_, 0.1f);
	if (ImGui::Button("Add")) {
		railPoints.emplace_back(RailPointData(newPoint_, newRotate_));
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
			ImGui::DragFloat3("point", &railPoints[std::stoi(selectIndex_)].pos.x, 0.1f);
			Quaternion::Debug_Gui(railPoints[std::stoi(selectIndex_)].rotate, "edit_rotate");
			//ImGui::DragFloat("twist", &railPoints[std::stoi(selectIndex_)].twist, 0.1f);
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

	newRail_->GetTransform()->SetTranslaion(newPoint_);
	newRail_->GetTransform()->SetQuaternion(newRotate_);
	newRail_->Update();
}

void RailPointEditer::Debug_Gui() {
	ImGui::Begin("railPoint");
	EditRail();

	if (ImGui::Button("Save")) {
		Save();
	}
	ImGui::End();
}
std::vector<Vector3> RailPointEditer::GetRailPoints() {
	return railIndexPoints_;
}
std::vector<Vector3> RailPointEditer::GetRailBasePoints() {
	std::vector<Vector3> result;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		result.push_back(railPoints[oi].pos);
	}
	return result;
}
std::vector<float> RailPointEditer::GetRailRotateZ() {
	return railIndexPointsRotateZ_;
}
#endif