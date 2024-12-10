#include "AdjustmentItem.h"
#include <iostream>
#include <fstream>
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

const std::string AdjustmentItem::kDirectoryPath_ = "./Game/Resources/GameData/AdjustmentItem/";
std::string AdjustmentItem::nowSceneName_ = "";

namespace fs = std::filesystem;

AdjustmentItem* AdjustmentItem::GetInstance() {
	static AdjustmentItem instance;
	return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::Init(const std::string& nowScene) {
	nowSceneName_ = nowScene;

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectoryPath_ + nowSceneName_ + "/");
	if (!std::filesystem::exists(kDirectoryPath_ + nowSceneName_ + "/")) {
		std::filesystem::create_directories(kDirectoryPath_ + nowSceneName_ + "/");
	}

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::Update() {
#ifdef _DEBUG
	if (!ImGui::Begin("AdjustValue", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (ImGui::Button("Reload")) {
		LoadAllFile();
	}
	
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::LoadAllFile() {

	std::filesystem::directory_iterator rootDir(kDirectoryPath_ + "/" + nowSceneName_ );
	for (const fs::directory_entry& entryDir : fs::directory_iterator(rootDir)) {
		if (entryDir.is_directory()) {
			// サブディレクトリの名前を取得
			const fs::path& subDirPath = entryDir.path();

			for (const fs::directory_entry& subEntry : fs::directory_iterator(subDirPath)) {
				// サブディレクトリ内のファイルパスを取得
				const fs::path& filePath = subEntry.path();
				// ファイル拡張子を取得
				std::string extension = filePath.extension().string();

				// .jsonファイル以外はスキップ
				if (extension.compare(".json") != 0) {
					continue;
				}

				Load(subDirPath.stem().string(), filePath.stem().string());
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Groupの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::AddGroup(const std::string& groupName, const json& jsonData) {
	std::string rootKey;
	if (jsonData.is_object() && !jsonData.empty()) {
		rootKey = jsonData.begin().key();
	}
	
	jsonMap_[groupName].items.emplace(rootKey, jsonData);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::Save(const std::string& groupName, const json& saveData) {
	// 最上位キーの取得
	if (saveData.is_object() && !saveData.empty()) {
		// 最上位のキーの名前をファイル名とする
		std::string rootKey = saveData.begin().key();
		// ファイルパスの作成
		std::string filePath = kDirectoryPath_ + nowSceneName_ + "/" + groupName + "/" + rootKey + ".json";

		// -------------------------------------------------
		// ↓ ディレクトリがなければ作成を行う
		// -------------------------------------------------
		std::filesystem::path dirPath = std::filesystem::path(kDirectoryPath_ + groupName);
		if (!std::filesystem::exists(dirPath)) {
			std::filesystem::create_directories(dirPath);
			std::cout << "Created directory: " << dirPath << std::endl;
		}

		// -------------------------------------------------
		// ↓ ファイルを開けるかのチェックを行う
		// -------------------------------------------------
		std::ofstream outFile(filePath);
		if (outFile.fail()) {
			std::string message = "Faild open data file for write\n";
			//Log(message);
			assert(0);
			return;
		}

		// -------------------------------------------------
		// ↓ ファイルに実際に書き込む
		// -------------------------------------------------
		outFile << std::setw(4) << saveData << std::endl;
		outFile.close();

		//Log("JSON data saved as: " + filePath + "\n");
	} else {
		//Log("Invalid or empty JSON data\n");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::Load(const std::string& groupName, const std::string& rootKey) {
	std::string filePath = kDirectoryPath_ + nowSceneName_ + "/" + groupName + "/" + rootKey + ".json";

	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + filePath + ".json";
		assert(0);
		return;
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// 値の追加
	GetInstance()->AddGroup(groupName, root);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　値を取得する
//////////////////////////////////////////////////////////////////////////////////////////////////

json AdjustmentItem::GetData(const std::string& groupName, const std::string& rootKey) {
	return GetInstance()->GetValue(groupName, rootKey);
}

json AdjustmentItem::GetValue(const std::string& groupName, const std::string& rootKey) {
	return jsonMap_[groupName].items[rootKey];
}
