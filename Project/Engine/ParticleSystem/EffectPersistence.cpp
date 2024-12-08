#include "EffectPersistence.h"
#ifdef _DEBUG
#include "Engine/Manager//ImGuiManager.h"
#endif

EffectPersistence* EffectPersistence::GetInstance() {
	static EffectPersistence instance;
	return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::Init() {
	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectoryPath_ + "Effect/");
	if (!std::filesystem::exists(kDirectoryPath_ + "Effect/")) {
		std::filesystem::create_directories(kDirectoryPath_ + "Effect/");
	}

	std::filesystem::path direEmitter(kDirectoryPath_ + "Emitter/");
	if (!std::filesystem::exists(kDirectoryPath_ + "Emitter/")) {
		std::filesystem::create_directories(kDirectoryPath_ + "Emitter/");
	}

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::Update() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　新しい作成
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::CreateGroup(const std::string& groupName) {
	data_[groupName];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　全ファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::LoadAllFile() {
	// Effect
	std::filesystem::directory_iterator dir_it(kDirectoryPath_ + "Effect");
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();
		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		// ファイル読み込み
		Load(true, filePath.stem().string());
	}

	// Emitter
	std::filesystem::directory_iterator dir_Emitter(kDirectoryPath_ + "Emitter");
	for (const std::filesystem::directory_entry& entry : dir_Emitter) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();
		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		// ファイル読み込み
		Load(false, filePath.stem().string());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　個別読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::Load(bool isEffect, const std::string& fileName) {
	std::string filePath;
	// 読み込むjsonファイルのフルパスを合成する
	if (isEffect) {
		filePath = kDirectoryPath_ + "Effect/" + fileName + ".json";
		effectNames_.push_back(fileName);
	} else {
		filePath = kDirectoryPath_ + "Emitter/" + fileName + ".json";
		emitterNames_.push_back(fileName);
	}
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループがあるかを検索
	json::iterator itGroup = root.find(fileName);
	// 未登録チェック
	assert(itGroup != root.end());

	// 各項目についての処理
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& itemName = itItem.key();
		// アイテム名を取得

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を取得
			uint32_t value = itItem->get<uint32_t>();
			SetValue(fileName, itemName, value);

			// float型
		} else if (itItem->is_number_float()) {
			// float型の値を取得
			double value = itItem->get<double>();
			SetValue(fileName, itemName, static_cast<float>(value));

			// Vector3
		} else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(fileName, itemName, value);

		} else if (itItem->is_array() && itItem->size() == 4) {
			// float型のjson配列登録
			Vector4 value = { itItem->at(0), itItem->at(1), itItem->at(2), itItem->at(3) };
			SetValue(fileName, itemName, value);

		} else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(fileName, itemName, value);
			// bool
		} else if (itItem->is_boolean()) {
			bool flag = itItem->get<bool>();
			SetValue(fileName, itemName, flag);

			// string
		} else if (itItem->is_string()) {
			std::string string = itItem->get<std::string>();
			SetValue(fileName, itemName, string);

			
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存
/////////////////////////////////////////////////////////////////////////////////////////////////

void EffectPersistence::Save(bool isEffect, const std::string& fileName) {
	// グループ検索
	std::map<std::string, Group>::iterator itGroup = data_.find(fileName);
	// 未登録チェック
	assert(itGroup != data_.end());

	json root;
	root = json::object();
	// jsonオブジェクト登録
	root[fileName] = json::object();

	// ------------------------------------------
	// ↓ jsonに書き込む用の変数にデータを入れる
	// ------------------------------------------
	for (std::map<std::string, Items>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Items& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[fileName][itemName] = std::get<uint32_t>(item.value);

			// float型
		} else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[fileName][itemName] = std::get<float>(item.value);

			// Vector3型
		} else if (std::holds_alternative<Vector4>(item.value)) {
			// Vector3型の値を登録
			Vector4 value = std::get<Vector4>(item.value);
			root[fileName][itemName] = json::array({ value.x, value.y, value.z, value.w });

		} else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[fileName][itemName] = json::array({ value.x, value.y, value.z });

		} else if (std::holds_alternative<Vector2>(item.value)) {
			// Vector3型の値を登録
			Vector2 value = std::get<Vector2>(item.value);
			root[fileName][itemName] = json::array({ value.x, value.y });

			// bool
		} else if (std::holds_alternative<bool>(item.value)) {
			root[fileName][itemName] = std::get<bool>(item.value);

		} else if (std::holds_alternative<std::string>(item.value)) {
			root[fileName][itemName] = std::get<std::string>(item.value);
		}
	}

	// ------------------------------------------
	// ↓ 書き込みを行う
	// ------------------------------------------
	// 書き込むJSONファイルのフルパス
	std::string filePath;
	if (isEffect) {
		filePath = kDirectoryPath_ + "Effect/" + fileName + ".json";
	} else {
		filePath = kDirectoryPath_ + "Emitter/" + fileName + ".json";
	}

	// 書き込み用のファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		std::string message = "Faild open data file for write";
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();

}
