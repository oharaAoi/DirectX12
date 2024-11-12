#include "RailPointEditer.h"
#include "Render.h"
#include "Game/Scene/GameScene.h"

RailPointEditer::RailPointEditer() {}
RailPointEditer::~RailPointEditer() {}

void RailPointEditer::Init() {
	points_.clear();
	pointsRotateZ_.clear();
	
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

	const uint32_t lerpNum = 6; // 補完するオブジェクトの個数
	for (uint32_t oi = 0; oi < railPoints.size() - 1; ++oi) {
		for (uint32_t index = 0; index < lerpNum; ++index) {
			float t = (float)index / (float)lerpNum;
			Vector3 pos = Vector3::Lerp(railPoints[oi].pos, railPoints[oi + 1].pos, t);
			Quaternion rotate = Quaternion::Slerp(railPoints[oi].rotate, railPoints[oi + 1].rotate, CallEasingFunc(easeType_, t));

			points_.push_back(pos);
			pointsRotateZ_.push_back(rotate);
		}
	}

	newRail_ = std::make_unique<BaseGameObject>();
	newRail_->Init();
	newRail_->SetObject("rail.obj");

	//newRail_->SetObjectAxis();
}

void RailPointEditer::Update() {
	points_.clear();
	pointsRotateZ_.clear();
	railIndexPoints_.clear();

	std::vector<Quaternion> copyRotate;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		copyRotate.push_back(railPoints[oi].rotate.Normalize());
	}

	//const size_t segmentCount = 200;
	//// 点からSpline曲線を引く
	//for (uint32_t oi = 0; oi < segmentCount; ++oi) {
	//	float t = (1.0f / static_cast<float>(segmentCount)) * oi;
	//	Vector3 pos = CatmullRomPosition(copyPos, t);
	//	railIndexPoints_.push_back(pos);
	//}

	//// 点からSpline曲線を引く
	//for (uint32_t oi = 0; oi < segmentCount; ++oi) {
	//	float t = (1.0f / static_cast<float>(segmentCount)) * oi;
	//	Quaternion rotate = Quaternion::CatmullRomRotate(copyRotate, t);
	//	railIndexPointsRotateZ_.push_back(Quaternion::GetYawFromQuaternion(rotate));
	//}

	const uint32_t lerpNum = 5; // 補完するオブジェクトの個数
	for (uint32_t oi = 0; oi < railPoints.size() - 1; ++oi) {
		for (uint32_t index = 0; index < lerpNum; ++index) {
			float t = (float)index / (float)lerpNum;
			Vector3 pos = Vector3::Lerp(railPoints[oi].pos, railPoints[oi + 1].pos, t);
			Quaternion rotate = Quaternion::Slerp(railPoints[oi].rotate, railPoints[oi + 1].rotate, CallEasingFunc(easeType_, t));

			points_.push_back(pos);
			pointsRotateZ_.push_back(rotate);
		}
	}

	for (uint32_t oi = 0; oi < pointsRotateZ_.size() - 1; ++oi) {
		pointsRotateZ_[oi] = Quaternion::Slerp(pointsRotateZ_[oi], pointsRotateZ_[oi + 1], CallEasingFunc(easeType_, 0.9f));
	}

	std::vector<Vector3> copyPos;
	for (uint32_t oi = 0; oi < railPoints.size(); ++oi) {
		copyPos.push_back(railPoints[oi].pos);
	}

	const size_t segmentCount = points_.size();
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
			sortedRailPoints[index] = item.value();  // D番号順に格納
		}
	}

	for (const auto& [index, value] : sortedRailPoints) {
		Vector3 pos = { value["pos"][0], value["pos"][1], value["pos"][2] };
		Quaternion rotate(value["rotate"][0], value["rotate"][1], value["rotate"][2], value["rotate"][3]);

		railPoints.emplace_back(RailPointData(pos, rotate));
	}
}

void RailPointEditer::InitLoad() {
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

const Vector3 RailPointEditer::GetPoints(size_t index) const {
	return points_[index];
}

const Quaternion RailPointEditer::GetPointsRotate(size_t index) const {
	return pointsRotateZ_[index];
}

std::vector<Vector3> RailPointEditer::GetRailPoints() {
	return railIndexPoints_;
}
std::vector<Vector3> RailPointEditer::GetPoints() {
	return points_;
}
std::vector<float> RailPointEditer::GetPointsRotate() {
	return std::vector<float>();
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

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void RailPointEditer::EditRail() {

	ShowEasingDebug(easeType_);

	// -------------------------------------------------
	// ↓ 新しい点を追加する
	// -------------------------------------------------
	ImGui::DragFloat3("newPoint", &newPoint_.x, 0.1f);
	
	// ------------------- X ------------------- //
	ImGui::Text("pos.x");
	ImGui::SameLine();
	Vector3 dire = Vector3::ZERO();
	Vector3 direDrag = Vector3::ZERO();
	if (ImGui::ArrowButton("x##Left", ImGuiDir_Left)) {
		dire -= newRotate_.MakeRight() * 0.05f;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	if (ImGui::DragFloat("##x", &direDrag.x, 0.02f)) {
		dire += newRotate_.MakeRight() * direDrag.x;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("x##Right", ImGuiDir_Right)) {
		dire += newRotate_.MakeRight() * 0.05f;
	}

	// ------------------- Y ------------------- //
	ImGui::Text("pos.y");
	ImGui::SameLine();
	if (ImGui::ArrowButton("y##Left", ImGuiDir_Left)) {
		dire -= newRotate_.MakeUp() * 0.05f;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	if (ImGui::DragFloat("##y", &direDrag.y, 0.02f)) {
		dire += newRotate_.MakeUp() * direDrag.y;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("y##Right", ImGuiDir_Right)) {
		dire += newRotate_.MakeUp() * 0.05f;
	}
	// ------------------- Z ------------------- //
	ImGui::Text("pos.z");
	ImGui::SameLine();
	if (ImGui::ArrowButton("z##Left", ImGuiDir_Left)) {
		dire -= newRotate_.MakeForward() * 0.05f;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	if (ImGui::DragFloat("##z", &direDrag.z, 0.02f)) {
		dire += newRotate_.MakeForward() * direDrag.z;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("z##Right", ImGuiDir_Right)) {
		dire += newRotate_.MakeForward() * 0.05f;
	}

	newPoint_ += dire;
	
	Quaternion::Debug_Gui(newRotate_, "new_rotate");

	//ImGui::DragFloat("newTwist", &newTwist_, 0.1f);
	if (ImGui::Button("Add")) {
		railPoints.emplace_back(RailPointData(newPoint_, newRotate_));
		pGameScene_->ResetRail();
		isAdd_ = true;
	}

	if (isAdd_) {
		ImGui::Text("preAdd");
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

			// ------------------- X ------------------- //
			ImGui::Text("selectPos.x");
			ImGui::SameLine();
			Vector3 direSelect = Vector3::ZERO();
			Vector3 direDragSelect = Vector3::ZERO();
			if (ImGui::ArrowButton("sx##Left", ImGuiDir_Left)) {
				direSelect -= railPoints[std::stoi(selectIndex_)].rotate.MakeRight() * 0.05f;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##sx", &direDragSelect.x, 0.02f)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeRight() * direDragSelect.x;
			}
			ImGui::SameLine();
			if (ImGui::ArrowButton("sx##Right", ImGuiDir_Right)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeRight() * 0.05f;
			}

			// ------------------- Y ------------------- //
			ImGui::Text("selectPos.y");
			ImGui::SameLine();
			if (ImGui::ArrowButton("sy##Left", ImGuiDir_Left)) {
				direSelect -= railPoints[std::stoi(selectIndex_)].rotate.MakeUp() * 0.05f;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##sy", &direDragSelect.y, 0.02f)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeUp() * direDragSelect.y;
			}
			ImGui::SameLine();
			if (ImGui::ArrowButton("sy##Right", ImGuiDir_Right)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeUp() * 0.05f;
			}
			// ------------------- Z ------------------- //
			ImGui::Text("selectPos.z");
			ImGui::SameLine();
			if (ImGui::ArrowButton("sz##Left", ImGuiDir_Left)) {
				direSelect -= railPoints[std::stoi(selectIndex_)].rotate.MakeForward() * 0.05f;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			if (ImGui::DragFloat("##sz", &direDragSelect.z, 0.02f)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeForward() * direDragSelect.z;
			}
			ImGui::SameLine();
			if (ImGui::ArrowButton("sz##Right", ImGuiDir_Right)) {
				direSelect += railPoints[std::stoi(selectIndex_)].rotate.MakeForward() * 0.05f;
			}

			railPoints[std::stoi(selectIndex_)].pos += direSelect;

			if (ImGui::Button("delte")) {
				railPoints.erase(railPoints.begin() + std::stoi(selectIndex_));
				pGameScene_->ResetRail();
			}
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
#endif