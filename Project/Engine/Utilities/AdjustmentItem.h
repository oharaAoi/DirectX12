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

	struct Items {
		std::variant<uint32_t, float, bool, Vector2, Vector3> value;
	};

	struct Group {
		std::map<std::string, Items> items;
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

	void CreateGroup(const std::string& groupName);

	void LoadAllFile();
	void Load(const std::string& fileName);
	void Save(const std::string& fileName);

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

private:

	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/AdjustmentItem/";
	std::map<std::string, Group> data_;
	std::string nowSceneName_;
};

template<typename T>
inline void AdjustmentItem::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = data_[groupName];
	// 新しい項目のデータを設定
	Items newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void AdjustmentItem::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = data_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

template<typename T>
inline T AdjustmentItem::GetValue(const std::string& groupName, const std::string& key) const {
	// 未登録チェック
	assert(data_.find(groupName) != data_.end());
	// グループの参照を取得
	const Group& group = data_.at(groupName);

	assert(group.items.find(key) != group.items.end());
	const Items& item = group.items.at(key);

	return std::get<T>(item.value);
}
