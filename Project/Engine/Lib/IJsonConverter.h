#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>

using json = nlohmann::json;

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
	virtual json ToJson() const = 0;

	/// <summary>
	/// json形式"から"変換する純粋仮想関数
	/// </summary>
	/// <param name="jsonData">: 任意のデータが格納されたjsonデータ</param>
	virtual void FromJson(const json& jsonData) = 0;

protected:

	json data_;

};

json toJson(const Vector2& v);

json toJson(const Vector3& v);




template <typename T>
inline void fromJson(const json& j, const std::string& name, T& value) {
	if constexpr (std::is_same_v<T, Vector3>) {
		// Vector3型に対する処理
		value.x = j.at(name).at("x").get<float>();
		value.y = j.at(name).at("y").get<float>();
		value.z = j.at(name).at("z").get<float>();
	} else if constexpr (std::is_same_v<T, float>) {
		// float型に対する処理
		value = j.at(name).get<float>();
	} else if constexpr (std::is_same_v<T, int>) {
		// int型に対する処理
		value = j.at(name).get<int>();
	}
}