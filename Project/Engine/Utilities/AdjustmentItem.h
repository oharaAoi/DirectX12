#pragma once
#include <iostream>
#include <fstream>
#include <variant>
#include <map>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"

using json = nlohmann::json;

class AdjustmentItem {
public:

	struct Group {
		std::unordered_map<std::string, json> items;
	};

public:

	AdjustmentItem() = default;
	~AdjustmentItem() = default;

	// シングルトン化
	AdjustmentItem(const AdjustmentItem&) = delete;
	AdjustmentItem& operator=(const AdjustmentItem&) = delete;

	static AdjustmentItem* GetInstance();

	void Init(const std::string& nowScene);
	void Update();

	void LoadAllFile();
	//void Load(const std::string& fileName);
	//void Save(const std::string& fileName);

	/// <summary>
	/// json項目を追加する
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="jsonData"></param>
	void AddGroup(const std::string& groupName, const json& jsonData);

private:

	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/AdjustmentItem/";
	std::string nowSceneName_;
	
	std::unordered_map<std::string, Group> jsonMap_;
};
