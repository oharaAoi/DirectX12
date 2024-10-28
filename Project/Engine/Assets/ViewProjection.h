#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/MyMath.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

struct ViewProjectionData {
	Matrix4x4 view;
	Matrix4x4 projection;
};

class ViewProjection {
public:

	ViewProjection();
	~ViewProjection();

	void Finalize();

	void Init(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* commandList) const;

	/// <summary>
	/// データをセットする
	/// </summary>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	void SetViewProjection(const Matrix4x4& view, const Matrix4x4& projection) {
		data_->view = view;
		data_->projection = projection;
	}

	const Matrix4x4 GetViewProjection() { return data_->view * data_->projection; }

	const Matrix4x4 GetViewMatrix() { return data_->view; }
	const Matrix4x4 GetProjectionMatrix() { return data_->projection; }

private:

	ComPtr<ID3D12Resource> cBuffer_;
	ViewProjectionData* data_;
};

