#include <Windows.h>
#include "Engine.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	// mainループ
	while (Engine::ProcessMessage()) {

	}

	return 0;
}