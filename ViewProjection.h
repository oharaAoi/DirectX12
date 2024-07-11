#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "DirectXUtils.h"
// math
#include "MyMath.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

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

private:

	Comptr<ID3D12Resource> cBuffer_;
	ViewProjectionData* data_;
};

