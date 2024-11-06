#include "BossAttackEditer.h"

BossAttackEditer::BossAttackEditer() {}
BossAttackEditer::~BossAttackEditer() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::Init() {
	controlPoint_.clear();
	controlPointObjects_.clear();

	segmentCount_ = 50;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Update() {
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		controlPointObjects_[index]->Update();

		if (index != selectIndex_) {
			controlPointObjects_[index]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	movePoints_.clear();
	if (controlPoint_.size() > 4) {
		for (uint32_t oi = 0; oi < segmentCount_; ++oi) {
			float t = (1.0f / static_cast<float>(segmentCount_)) * oi;
			Vector3 pos = CatmullRomPosition(controlPoint_, t);
			movePoints_.push_back(pos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackEditer::Draw() const {
	// -------------------------------------------------
	// ↓ 線の描画
	// -------------------------------------------------
	if (movePoints_.size() > 4) {
		Engine::SetPipeline(PipelineType::PrimitivePipeline);
		for (size_t index = 0; index < movePoints_.size() - 1; ++index) {
			Render::DrawLine(movePoints_[index], movePoints_[index + 1], Vector4(1, 1, 1, 1));
		}
	}

	// -------------------------------------------------
	// ↓ pointのオブジェクトの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		controlPointObjects_[index]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃に関するファイルを読み込む
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::LoadAllFile(const std::string& directoryPath) {
	attackMoveMap_.clear();
	attackFileNames_.clear();
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		std::string fileName = entry.path().stem().string();
		LoadFile(directoryPath, fileName);
	}
}

void BossAttackEditer::LoadFile(const std::string& directoryPath, const std::string& fileName) {
	std::string filePath = directoryPath + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	std::vector<HandTransform> handTransforms;
	for (auto& [topKey, nestedData] : root.items()) {
		for (auto& [key, value] : nestedData.items()) {
			Vector3 scale = { value["scale"][0], value["scale"][1], value["scale"][2] };
			Vector4 rotate = { value["rotate"][0], value["rotate"][1], value["rotate"][2], value["rotate"][3] };
			Vector3 translate = { value["translate"][0], value["translate"][1], value["translate"][2] };

			auto& newTransform = handTransforms.emplace_back(HandTransform());
			newTransform.scale = scale;
			newTransform.rotate = Quaternion(rotate.x, rotate.y, rotate.z, rotate.w);
			newTransform.translate = translate;
		}
	}

	attackMoveMap_.try_emplace(fileName, std::move(handTransforms));
	attackFileNames_.push_back(fileName);
}

#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ポイントの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::AddPoint() {
	ImGui::DragFloat3("newPoint", &newPoint_.x, 0.1f);
	if (ImGui::Button("AddPoint")) {
		controlPoint_.push_back(newPoint_);
		auto& newObject = controlPointObjects_.emplace_back(std::make_unique<BaseGameObject>());
		newObject->Init();
		newObject->SetObject("cube.obj");
		newObject->GetTransform()->SetTranslaion(newPoint_);
		newObject->GetTransform()->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	}
}

void BossAttackEditer::AddPoint(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	auto& newPoint = handTransforms_.emplace_back(HandTransform());
	newPoint.scale = scale;
	newPoint.rotate = rotate;
	newPoint.translate = translate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ポイントの削除
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::DeletePoint(const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	std::vector<Vector2> objectPos;
	for (size_t index = 0; index < controlPointObjects_.size(); ++index) {
		// objectのscreen座標を求める
		Vector3 objectScreen = Transform(Vector3::ZERO(), controlPointObjects_[index]->GetTransform()->GetWorldMatrix() * vpvpMat);
		Vector2 objectScreenPos = Vector2(objectScreen.x, objectScreen.y);
		objectPos.push_back(objectScreenPos);
	}

	for(uint32_t index = 0; index < controlPointObjects_.size(); ++index)
	// 長さを取って距離が近かったら
	if ((mousePos - objectPos[index]).Length() < 20.0f) {
		if (Input::IsTriggerMouse(0)) {
			selectIndex_ = index;
			controlPointObjects_[index]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	// 選択されたポイントを削除する
	ImGui::Text("selectIndex : %d", selectIndex_);
	if (Input::IsTriggerKey(DIK_BACKSPACE)) {
		if (selectIndex_ >= 0 && selectIndex_ < controlPointObjects_.size()) {
			controlPointObjects_.erase(controlPointObjects_.begin() + selectIndex_);
		}

		if (selectIndex_ >= 0 && selectIndex_ < controlPoint_.size()) {
			controlPoint_.erase(controlPoint_.begin() + selectIndex_);
		}

		selectIndex_ = 999;
	}
}

void BossAttackEditer::Debug_Gui(const std::string& directoryPath) {
	ImGui::Text("handTransforms : %d", (int)handTransforms_.size());

	ImGui::InputText(".json##fileName", &fileName_[0], sizeof(char) * 64);

	// 攻撃のファイルを選ぶ
	SelectAttack();

	if (ImGui::Button("Save")) {
		SaveAttack(directoryPath);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃に関するファイルを保存する
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossAttackEditer::SaveAttack(const std::string& directoryPath) {
	json root;

	// handTransformsの先頭から順でファイルに保存する
	for (uint32_t index = 0; index < handTransforms_.size(); ++index) {
		std::string id = "handTransform" + std::to_string(index);

		json scale = json::array({ handTransforms_[index].scale.x,handTransforms_[index].scale.y, handTransforms_[index].scale.z });
		json rotate = json::array({ handTransforms_[index].rotate.x,handTransforms_[index].rotate.y, handTransforms_[index].rotate.z, handTransforms_[index].rotate.w });
		json pos = json::array({ handTransforms_[index].translate.x,handTransforms_[index].translate.y, handTransforms_[index].translate.z });

		root[fileName_.c_str()][id] = {
			{"scale", scale},
			{"rotate", rotate},
			{"translate", pos}
		};
	}

	// 実際にファイルに保存する
	std::string filePath = directoryPath + fileName_.c_str() + ".json";

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
void BossAttackEditer::SelectAttack() {
	if (!attackFileNames_.empty()) {
		selectFileName_ = attackFileNames_[selectIndex_];
	}
	
	if (ImGui::BeginCombo(".json##", &selectFileName_[0], ImGuiComboFlags_HeightLargest)) {
		for (int i = 0; i < attackFileNames_.size(); i++) {
			const bool isSelected = (selectIndex_ == i);
			if (ImGui::Selectable(attackFileNames_[selectIndex_].c_str(), isSelected)) {
				selectIndex_ = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
#endif 