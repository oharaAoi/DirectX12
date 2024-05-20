#include "DirectXCommon.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

DirectXCommon::~DirectXCommon() {

}

DirectXCommon* DirectXCommon::GetInstacne() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize(WinApp* win, int32_t backBufferWidth, int32_t backBufferHeight) {
	assert(win);
	winApp_ = win;
	kClientWidth_ = backBufferWidth;
	kClientHeight_ = backBufferHeight;
}

void DirectXCommon::Finalize() {
}

void DirectXCommon::CreateDXGI() {
	dxgiFactory_ = nullptr;
	// HRESULはwidows系のエラーコードであり
	// 関数が成功したかどうかをSUCCEEDEマクロ判定で判定できる
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));
}

void DirectXCommon::SetUseGPU() {
	HRESULT hr;
	// 使用するアダプタ用の変数。最初にnullptrを入れておく
	useAdapter_ = nullptr;
	// 良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
															  DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {

		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); // 取得出来なかった時用
		// ソフトウェアのアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// 採用したアダプタの情報の情報をログに出力.wstrinfgの方
			Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}

		useAdapter_ = nullptr; // ソフトウェアアダプタの場合は見なかったことにする
	}

	// 適切なアダプタが見つからなかったら軌道できない
	assert(useAdapter_ != nullptr);
}
