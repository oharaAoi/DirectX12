#include "WinApp.h"

//#include "ImGuiManager.h"

WinApp* WinApp::GetInstance(){
    static WinApp instance;
    return &instance;
}

/*==========================================================================
    ウィンドウプロシージャ
==========================================================================*/
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
#ifdef _DEBUG
	if (ImGuiManager::ImGuiHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
#endif // _DEBUG


	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破壊された
	case WM_DESTROY:
		// OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinApp::~WinApp(){
	Finalize();
}

/*==========================================================================
	windowを生成する
==========================================================================*/
void WinApp::CreateGameWindow(){
	hwnd_ = nullptr;
	//windowClass -------------------------------------------
	wc.lpfnWndProc = WindowProc;
	// ウィンドウのクラス名
	wc.lpszClassName = L"CG2";
	// インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc);

	//windowSize ---------------------------------------------
	// クライアント領域のサイズ
	const int32_t kClientWidth = kWindowWidth_;
	const int32_t kClientHeight = kWindowHeight_;

	// ウィンドウサイズをあらわs構造体にクライアント領域を入れる
	RECT wrc = { 0,0, kClientWidth, kClientHeight };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	
	// windowの生成 ---------------------------------------------
	hwnd_ = CreateWindow(
		wc.lpszClassName,		// 利用するクラス名
		L"2205_",					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// よく見るウィンドウスタイル
		CW_USEDEFAULT,			// 表示x座標
		CW_USEDEFAULT,			// 表示y座標
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wc.hInstance,			// インスタンスハンドル
		nullptr					// オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

void WinApp::SetFullScreen(bool fullscreen) {
	if (isFullscreen_ != fullscreen) {
		if (fullscreen) {
			// 元の状態を覚えておく
			GetWindowRect(hwnd_, &windowRect_);

			// 仮想フルスクリーン化
			SetWindowLong(
				hwnd_, GWL_STYLE,
				windowStyle_ &
				~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

			RECT fullscreenRect{ 0 };
			HMONITOR monitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
			MONITORINFO info;
			info.cbSize = sizeof(info);
			GetMonitorInfo(monitor, &info);
			fullscreenRect.right = info.rcMonitor.right - info.rcMonitor.left;
			fullscreenRect.bottom = info.rcMonitor.bottom - info.rcMonitor.top;

			SetWindowPos(
				hwnd_, HWND_TOPMOST, fullscreenRect.left, fullscreenRect.top, fullscreenRect.right,
				fullscreenRect.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);
			ShowWindow(hwnd_, SW_MAXIMIZE);

		} else {
			// 通常ウィンドウに戻す
			SetWindowLong(hwnd_, GWL_STYLE, windowStyle_);

			SetWindowPos(
				hwnd_, HWND_NOTOPMOST, windowRect_.left, windowRect_.top,
				windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			ShowWindow(hwnd_, SW_NORMAL);
		}
	}

	isFullscreen_ = fullscreen;
}

//===============================================================================================================
//	
//===============================================================================================================
bool WinApp::ProcessMessage(){
	MSG msg{};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 終了メッセージが来たらループを抜ける
	if (msg.message != WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize(){
	CloseWindow(hwnd_);
}
