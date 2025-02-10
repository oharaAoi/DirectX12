#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
// DirectX
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/MyMath.h"

class Phong {
public: // データ構造体

	struct PhongDate {
		Vector4 color;
		Vector3 lightDirection;
		float pad;
		Vector3 eyePos;
		float intensity_;
	};

public:

	Phong();
	~Phong();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Init(ID3D12Device* device);

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ライトをセットする
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="rootParameterIndex"></param>
	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);

public:

	void SetEyePos(const Vector3& pos) { lightDate_->eyePos = pos; }

private:

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;

	Vector4 color_;
	Vector3 direction_;
	float intensity_;

	PhongDate* lightDate_;

};