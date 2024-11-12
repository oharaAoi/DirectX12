#include "Convert.h"

std::wstring ConvertWString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str){
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

std::wstring ConvertToWideString(const char* narrowStr) {
	// 必要なワイド文字列のサイズを取得（null終端も含むために -1 を渡す）
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);

	// 必要なサイズのwstringを確保
	std::wstring wideStr(size_needed, 0);

	// マルチバイト文字列をワイド文字列に変換
	MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, &wideStr[0], size_needed);

	return wideStr;
}
