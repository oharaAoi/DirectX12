#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include <nlohmann/json.hpp>
#include <string>
#include <assert.h>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <Engine/Utilities/DirectXUtils.h>

using json = nlohmann::json;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// このクラスは構造体やクラスのメンバ変数をjson形式に変換(逆もまた)する
/// 関数の定義を提供するクラス
/// </summary>
class IJsonConverter {

public:
	virtual ~IJsonConverter() = default;

	/// <summary>
	/// json形式"に"変換する純粋仮想関数
	/// </summary>
	/// <returns>: jsonファイル形式に格納されたデータ</returns>
	virtual json ToJson(const std::string id) const = 0;

	/// <summary>
	/// json形式"から"変換する純粋仮想関数
	/// </summary>
	/// <param name="jsonData">: 任意のデータが格納されたjsonデータ</param>
	virtual void FromJson(const json& jsonData) = 0;

protected:

};

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline json toJson(const T& v) {
	if constexpr (std::is_same_v<T, Vector4>) {
		// Vector4型に対する処理
		return json{ {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
	} else if constexpr (std::is_same_v<T, Vector3>) {
		// Vector3型に対する処理
		return json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	} else if constexpr (std::is_same_v<T, Vector2>) {
		// Vector2型に対する処理
		return json{ {"x", v.x}, {"y", v.y} };
	} else if constexpr (std::is_same_v<T, float>) {
		// float型に対する処理
		return v;
	} else if constexpr (std::is_same_v<T, int>) {
		// int型に対する処理
		return v;
	} else if constexpr (std::is_same_v<T, uint32_t>) {
		// uint32_t型に対する処理
		return v;
	} else if constexpr (std::is_same_v<T, std::string>) {
		// std::string型に対する処理
		return v;
	} else {
		assert(false && "Unsupported type in toJson");
	}
}

template <typename T>
inline void fromJson(const json& j, const std::string& name, T& value) {
	if (j.is_object()) {
		// JSON オブジェクト内で最初のキーを取得
		auto rootKey = j.begin().key(); // 最上位キーを取得

		// jsonにnameが含まれていたら
		if (j.at(rootKey).contains(name)) {
			if constexpr (std::is_same_v<T, Vector3>) {
				// Vector3型に対する処理
				value.x = j.at(rootKey).at(name).at("x").get<float>();
				value.y = j.at(rootKey).at(name).at("y").get<float>();
				value.z = j.at(rootKey).at(name).at("z").get<float>();
			} else if constexpr (std::is_same_v<T, Vector2>) {
				// Vector2型に対する処理
				value.x = j.at(rootKey).at(name).at("x").get<float>();
				value.y = j.at(rootKey).at(name).at("y").get<float>();
			} else if constexpr (std::is_same_v<T, float>) {
				// float型に対する処理
				value = j.at(rootKey).at(name).get<float>();
			} else if constexpr (std::is_same_v<T, int>) {
				// int型に対する処理
				value = j.at(rootKey).at(name).get<int>();
			} else if constexpr (std::is_same_v<T, uint32_t>) {
				// uint32_t型に対する処理
				value = j.at(rootKey).at(name).get<uint32_t>();
			} else if constexpr (std::is_same_v<T, std::string>) {
				// std::string型に対する処理
				value = j.at(rootKey).at(name).get<std::string>();
			}
		} else {
			// json内にnameが存在していなかったら
			std::string erroeLog = "not contains jsonData  : " + name;
			Log(erroeLog);
			assert(false && "Name is missing in the JSON");
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　json形式のデータを構築するようのクラス
//////////////////////////////////////////////////////////////////////////////////////////////////

class JsonBuilder {
public:

	JsonBuilder(const std::string& name) : hierarchyName_(name) {};
	~JsonBuilder() = default;

	/// <summary>
	/// json項目を追加する関数
	/// </summary>
	/// <typeparam name="T">: 任意の型</typeparam>
	/// <param name="key">: 値に対応する名前</param>
	/// <param name="value">] 値</param>
	/// <returns>: json型にkeyとvalueのペアが登録される</returns>
	template <typename T>
	JsonBuilder& Add(const std::string& key, const T& value) {
		auto jsonValue = toJson(value);
		jsonData_[hierarchyName_][key] = std::move(jsonValue);
		return *this;// 自分自身を返す
	}

	/// <summary>
	/// 構築を実行する
	/// </summary>
	/// <returns></returns>
	json Build() const {
		return std::move(jsonData_);
	}

private:

	json jsonData_;
	std::string hierarchyName_;
};
