#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Engine/Components/Attribute/AttributeGui.h"

using json = nlohmann::json;

class JsonItems :
	public AttributeGui {
public:

	struct Group {
		std::unordered_map<std::string, json> items;
	};

public:

	JsonItems() = default;
	~JsonItems() = default;

	// シングルトン化
	JsonItems(const JsonItems&) = delete;
	JsonItems& operator=(const JsonItems&) = delete;

	static JsonItems* GetInstance();

	void Init(const std::string& nowScene);
	void Update();

	/// <summary>
	/// jsonファイルに保存する
	/// </summary>
	/// <param name="groupName">: ファイルを保存するフォルダ名</param>
	/// <param name="saveData">: 保存するデータ</param>
	static void Save(const std::string& groupName, const json& saveData);

	/// <summary>
	/// jsonファイルを読み込む
	/// </summary>
	/// <param name="groupName">: ファイルの保存されたフォルダ名</param>
	/// <param name="rootKey">: 読み込むファイル名</param>
	static void Load(const std::string& groupName, const std::string& rootKey);

	/// <summary>
	/// 値の取得(他クラスで呼び出し用)
	/// </summary>
	/// <param name="groupName">: ファイルの保存されたフォルダ名</param>
	/// <param name="rootKey">: 取得するファイル名</param>
	/// <returns>json型を返す</returns>
	static json GetData(const std::string& groupName, const std::string& rootKey);

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private :

	/// <summary>
	/// すべてのファイルを読み込む
	/// </summary>
	void LoadAllFile();

	/// <summary>
	/// json項目を追加する
	/// </summary>
	/// <param name="groupName">: ファイルを保存するフォルダ名</param>
	/// <param name="jsonData">: 保存するデータ</param>
	void AddGroup(const std::string& groupName, const json& jsonData);

	/// <summary>
	/// 値の取得(自クラスのみで呼び出し)
	/// </summary>
	/// <param name="groupName">: ファイルの保存されたフォルダ名</param>
	/// <param name="rootKey">: 取得するファイル名</param>
	/// <returns>json型を返す</returns>
	json GetValue(const std::string& groupName, const std::string& rootKey);

private:
	
	static const std::string kDirectoryPath_;
	static std::string nowSceneName_;
	
	std::unordered_map<std::string, Group> jsonMap_;
};
