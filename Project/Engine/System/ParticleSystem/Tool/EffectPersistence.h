#pragma once
#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Lib/Math/Vector3.h"
#include "Engine/Lib/Math/Vector4.h"

using json = nlohmann::json;

/// <summary>
/// Effectの保存と読み込みを管理するクラス
/// </summary>
class EffectPersistence {
public:

	struct Items {
		std::variant<uint32_t, float, bool, Vector2, Vector3, Vector4, std::string> value;
	};

	struct Group {
		std::map<std::string, Items> items;
	};

	EffectPersistence() = default;
	~EffectPersistence() = default;

	// シングルトン化
	EffectPersistence(const EffectPersistence&) = delete;
	EffectPersistence& operator=(const EffectPersistence&) = delete;

	static EffectPersistence* GetInstance();

	void Init();
	void Update();

	void CreateGroup(const std::string& groupName);

	void LoadAllFile();
	void Load(bool isEffect, const std::string& fileName);
	void Save(bool isEffect, const std::string& fileName);

	std::vector<std::string>& GetEffectNames() { return effectNames_; }
	std::vector<std::string>& GetEmitterNames() { return emitterNames_; }

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

	uint32_t GetItemsSize(const std::string& groupName) { return static_cast<uint32_t>(data_[groupName].items.size()); }

private:

	const std::string& kDirectoryPath_ = "./Game/Resources/Effects/";
	std::map<std::string, Group> data_;

	std::vector<std::string> effectNames_;
	std::vector<std::string> emitterNames_;
	
};

template<typename T>
inline void EffectPersistence::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = data_[groupName];
	// 新しい項目のデータを設定
	Items newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void EffectPersistence::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = data_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

template<typename T>
inline T EffectPersistence::GetValue(const std::string& groupName, const std::string& key) const {
	// 未登録チェック
	assert(data_.find(groupName) != data_.end());
	// グループの参照を取得
	const Group& group = data_.at(groupName);

	assert(group.items.find(key) != group.items.end());
	const Items& item = group.items.at(key);

	return std::get<T>(item.value);
}


