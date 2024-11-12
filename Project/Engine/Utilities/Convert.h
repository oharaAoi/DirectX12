#pragma once
#include <format>
#include <Windows.h>

/*================================================================================================
DirectX12が返してくるstd::wstringからstd::stingに変換する関数
==================================================================================================*/

/// <summary>
/// <summary>
/// std::wstringで返す
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertWString(const std::string& str);

/// <summary>
///  std::stringで返す
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string ConvertString(const std::wstring& str);


std::wstring ConvertToWideString(const char* narrowStr);