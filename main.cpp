#include <Windows.h>
#include "Engine.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	Mesh::Vertices vertex = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};

	std::unique_ptr<Triangle> triangle = Engine::CreateTriangle(vertex);

	// mainループ
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		Engine::DrawTriangle(triangle.get());

		Engine::EndFrame();
	}

	return 0;
}