#include "AdjustmentItem.h"
#ifdef _DEBUG
#include "Engine/Manager//ImGuiManager.h"
#endif

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
	std::filesystem::directory_iterator dir_it(kDirectoryPath_ + "/" + nowSceneName_ + "/");
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();
		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		//// ファイル読み込み
		//Load(filePath.stem().string());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存を行う
//////////////////////////////////////////////////////////////////////////////////////////////////



void AdjustmentItem::AddGroup(const std::string& groupName, const json& jsonData) {
	std::string id = jsonData;
	json data = { groupName, jsonData };
	//jsonMap_[groupName] = { groupName, jsonData };
}
