#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// winapp
#include "WinApp.h"
// utilities
#include "Convert.h"
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class DirectXCommon {
public:

	DirectXCommon() = default;
	~DirectXCommon();
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DirectXCommon* GetInstacne();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win, int32_t backBufferWidth, int32_t backBufferHeight);

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

public:

	/// <summary>
	/// DXGIFactoryの生成
	/// </summary>
	void CreateDXGI();

	/// <summary>
	/// 使用するGUPを決める
	/// </summary>
	void SetUseGPU();

private:

	WinApp* winApp_ = nullptr;
	int32_t kClientWidth_;
	int32_t kClientHeight_;

private:

	Comptr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Comptr<IDXGIAdapter4> useAdapter_ = nullptr;

};

