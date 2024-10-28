#pragma once
#include <Windows.h>
#include <cstdint>
#include "Enviroment.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WinApp{
public: // 静的メンバ変数
	// ウィンドウサイズ
	//static const int32_t kWindowWidth = 1280;	// 横幅
	//static const int32_t kWindowHeight = 720;	// 縦幅
	// ウィンドウクラス名
	static const wchar_t kWindowClassName[];

public: // 静的メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロージャー
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	WinApp() = default;
	~WinApp();
	WinApp(const WinApp& obj) = delete;
	const WinApp& operator=(const WinApp&) = delete;

	void CreateGameWindow();

	void SetFullScreen(bool fullscreen);

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; }

	/// <summary>
	/// windowクラスの取得
	/// </summary>
	/// <returns></returns>
	WNDCLASS GetWNDCLASS() const { return wc; }

	void Finalize();

private: // メンバ変数
	// window関連
	HWND hwnd_ = nullptr; // ウィンドウハンドル
	WNDCLASS wc{};
	RECT windowRect_;
	UINT windowStyle_ = WS_OVERLAPPEDWINDOW;

	bool isFullscreen_;
	int32_t windowWidth_;
	int32_t windowHeight_;
};

