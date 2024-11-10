#include "EnemyManager.h"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Init() {
	enemyList_.clear();
	isPop_ = true;

	newEnmey_ = std::make_unique<Enemy>();
	newEnmey_->Init();

	std::filesystem::path dire(directoryPath_);
	if (!std::filesystem::exists(directoryPath_)) {
		std::filesystem::create_directories(directoryPath_);
	}

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Update() {
	if (isPop_) {
		popTime_ += GameTimer::DeltaTime();

		if (popTime_ > 6.0f) {
			AddList(playerPos_);
			popTime_ = 0.0f;
		}
	}

	// 生存確認
	enemyList_.remove_if([](auto& enemy) {return !enemy->GetIsAlive(); });

	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemyList_.begin(); itr != enemyList_.end();) {
		(*itr)->Update();
		++itr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Draw() const {
	for (std::list<std::unique_ptr<Enemy>>::const_iterator itr = enemyList_.begin(); itr != enemyList_.end();) {
		(*itr)->Draw();
		++itr;
	}

	for (auto& popEnemy : popEnemies_) {
		popEnemy->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Enemyの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::AddList(const Vector3& popPos) {
	Vector3 pos = popPos + (playerForward_ * 80.0f);
	
	Vector3 randomPos = Vector3(
		RandomFloat(-5.0f, 5.0f),
		RandomFloat(-5.0f, 5.0f),
		RandomFloat(-5.0f, 5.0f)
	);
	pos += randomPos;

	auto& newEnemy = enemyList_.emplace_back(std::make_unique<Enemy>());
	newEnemy->Init();
	newEnemy->GetTransform()->SetTranslaion(pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Enemyの読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Load(const std::string& fileName) {
	std::string filePath = directoryPath_ + fileName + ".json";
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

	std::vector<NewEnmeyData> newEnemyData;
	uint32_t popIndex = 0;

	for (auto& [topKey, nestedData] : root.items()) {
		for (auto& [key, value] : nestedData.items()) {
			if (key == "popIndex") {
				popIndex = value;
			}
		}
	}

	for (auto& [topKey, nestedData] : root.items()) {
		for (auto& [key, value] : nestedData.items()) {
			if (key != "popIndex") {
				Vector3 translate = { value["translate"][0], value["translate"][1], value["translate"][2] };
				Vector3 velocity = { value["velocity"][0], value["velocity"][1], value["velocity"][2] };
				int typeInt = value["type"];
				EnemyType type = (EnemyType)typeInt;

				newEnemyData.emplace_back(NewEnmeyData(translate, velocity, type));
			}
		}
	}

	popEnemyData_.try_emplace(popIndex, std::move(newEnemyData));
}

void EnemyManager::LoadAllFile() {
	popEnemyData_.clear();
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath_)) {
		std::string fileName = entry.path().stem().string();
		Load(fileName);
	}
}

#ifdef _DEBUG
void EnemyManager::Debug_Gui() {
	if (ImGui::TreeNode("EnemyManager")) {
		ImGui::Begin("EnmeyManager");
		ImGui::Checkbox("isPop", &isPop_);

		Edit();

		ImGui::End();
		ImGui::TreePop();
	}
}
void EnemyManager::Edit() {
	Edit_Pop();

	// 出現タイミングを変更
	ImGui::InputScalar("popIndex", ImGuiDataType_U32, &popIndex_);

	if (ImGui::Button("Save")) {
		Save();
	}
	ImGui::SameLine();
	ImGui::InputText(".json##enemyDataFile", &enemyDataFile_[0], sizeof(char) * 64);
}

void EnemyManager::Edit_Pop() {
	if (ImGui::TreeNode("Edit_Pop")) {
		if (ImGui::Button("pop")) {
			auto& newObj = popEnemies_.emplace_back(std::make_unique<Enemy>());
			newObj->Init();
		}

		uint32_t index = 0;
		for (auto& popEnemy : popEnemies_) {
			std::string id = "enemy_" + std::to_string(index);
			if (ImGui::TreeNode(id.c_str())) {
				popEnemy->Debug_Gui();
				popEnemy->Update();
				ImGui::TreePop();
			}

			++index;
		}
		
		ImGui::TreePop();
	}
}
void EnemyManager::Save() {
	json root;

	root[enemyDataFile_.c_str()]["popIndex"] = popIndex_;
	// handTransformsの先頭から順でファイルに保存する
	uint32_t index = 0;
	for (auto& popEnemy : popEnemies_) {
		std::string id = "enemyData_" + std::to_string(index);

		Vector3 translate = popEnemy->GetTranslate();
		Vector3 velocity = popEnemy->GetVelocity();

		json pos = json::array({ translate.x,translate.y, translate.z });
		json saveVelocity = json::array({ velocity.x,velocity.y, velocity.z });

		root[enemyDataFile_.c_str()][id] = {
			{"translate", pos},
			{"velocity", saveVelocity},
			{"type", popEnemy->GetEnemyType()},
		};

		++index;
	}

	// 実際にファイルに保存する
	std::string filePath = directoryPath_ + enemyDataFile_.c_str() + ".json";

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
#endif