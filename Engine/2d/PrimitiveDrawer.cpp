#include "PrimitiveDrawer.h"

void PrimitiveDrawer::Init(ID3D12Device* device) {
	// ---------------------------------------------------------------
	// ↓Vertexの設定
	// ---------------------------------------------------------------
	// VertexBufferViewを作成する 
	vertexBuffer_ = CreateBufferResource(device, sizeof(PrimitiveData) * kMaxLineCount);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(PrimitiveData) * kMaxLineCount);
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(PrimitiveData);
	// Resourceにデータを書き込む 
	primitiveData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&primitiveData_));
	//std::memcpy(vertexData_, , sizeof(Vector4) * kMaxLineCount);

	// ---------------------------------------------------------------
	// ↓Indexの設定
	// ---------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * kMaxLineCount);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * kMaxLineCount);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	for (uint32_t oi = 0; oi < kMaxLineCount; oi++) {
		indexData_[oi] = oi;
	}

	// ---------------------------------------------------------------
	// ↓colorの設定
	// ---------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(PrimitiveData) * kMaxLineCount);
	materialData_ = nullptr;
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//*materialData_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	for (uint32_t oi = 0; oi < kMaxLineCount; oi++) {
		materialData_[oi] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ---------------------------------------------------------------
	// ↓wvpの設定
	// ---------------------------------------------------------------
	wvpBuffer_ = CreateBufferResource(device, sizeof(Matrix4x4));
	wvpData_ = nullptr;
	wvpBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();

	useIndex_ = 0;
}

void PrimitiveDrawer::Finalize() {
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	wvpBuffer_.Reset();
}

void PrimitiveDrawer::Draw(ID3D12GraphicsCommandList* commandList, const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat) {
	// 使用する頂点のインデックスの更新
	size_t indexVertex = useIndex_;
	size_t materialIndex = (useIndex_) - 1;

	if (useIndex_ == 0) {
		materialIndex = 0;
	}

	// 頂点の設定
	primitiveData_[useIndex_].pos = { p1.x, p1.y, p1.z, 1 };
	primitiveData_[useIndex_ + 1].pos = { p2.x, p2.y, p2.z, 1 };

	// 色の設定
	primitiveData_[useIndex_].color = color;
	primitiveData_[useIndex_ + 1].color = color;

	*wvpData_ = vpMat;

	// コマンドリストの設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpBuffer_->GetGPUVirtualAddress());

	// インデックスを使用して線を描画
	commandList->DrawIndexedInstanced(kVertexCountLine, 1, static_cast<UINT>(indexVertex), 0, 0);
	
	useIndex_ += 2;
}
