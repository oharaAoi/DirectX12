#pragma once
#include <cstdint>

enum class CheckType {
	HAS,		// 含まれているか
	EQUAL		// 完全一致するか
};

/// <summary>
/// 判定を行う
/// </summary>
/// <param name="bitA">: 比較対象A</param>
/// <param name="bitB">: 比較対象B</param>
/// <param name="type">: 比較する種類</param>
/// <returns>: true | false</returns>
bool CheckBit(uint32_t bitA, uint32_t bitB, CheckType type);

// 含まれているか
bool HasBit(uint32_t bitA, uint32_t bitB);

// 完全に一致するか
bool EqualBit(uint32_t bitA, uint32_t bitB);
