#include "Trail.h"
#include "Engine/Engine.h"

Trail::Trail() {
}

Trail::~Trail() {
	verticesData_.clear();
	indices_.clear();
	materialBuffer_.Reset();
	indexBuffer_.Reset();
	vertexBuffer_.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trail::Init() {
	ID3D12Device* device = Engine::GetDevice();

	// ---------------------------------------------------------------
	// ↓Vetrtexの設定
	// ---------------------------------------------------------------
	// VertexBufferViewを作成する
	vertexBuffer_ = CreateBufferResource(device, kMaxTrailNum_ * (sizeof(VertexData) * 4));
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = kMaxTrailNum_ * (sizeof(VertexData) * 4);
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// ---------------------------------------------------------------
	// ↓indexの設定
	// ---------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * (kMaxTrailNum_ * 6));
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * (kMaxTrailNum_ * 6));
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexNum_ = (kMaxTrailNum_ * 6);

	vertexBuffer_->Unmap(0, nullptr);
	indexBuffer_->Unmap(0, nullptr);

	// ---------------------------------------------------------------
	// ↓ materialの設定
	// ---------------------------------------------------------------

	materialBuffer_ = CreateBufferResource(device, sizeof(MaterialData));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&material_));

	// ---------------------------------------------------------------
	// ↓ transformの設定
	// ---------------------------------------------------------------
	transform_ = Engine::CreateWorldTransform();

	nowTrailCount_ = 0;

	rect_.leftTop = { -1, -1, 0 };
	rect_.rightTop = { 1, -1, 0 };
	rect_.leftBottom = { -1, 1, 0 };
	rect_.rightBottom = { 1, 1, 0 };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trail::Update() {
	
	std::vector<bool> toDelete(verticesData_.size(), false);

	if (!verticesData_.empty()) {
		for (uint32_t oi = 0; oi < verticesData_.size() - 1; ++oi) {
			verticesData_[oi].color.w -= GameTimer::DeltaTime() * 3.0f;
		}
	}

	
	memcpy(vertexData_, verticesData_.data(), sizeof(VertexData) * verticesData_.size());
	memcpy(indexData_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trail::Draw(const ViewProjection* viewprojection) const {
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();

	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());

	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, "white.png", 1);
	viewprojection->BindCommandList(commandList);

	commandList->DrawIndexedInstanced(indexNum_, 1, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trail::AddTrail(const Matrix4x4& mat) {
	Vector3 translate = Transform(Vector3::ZERO(), mat);
	
	time_ += GameTimer::DeltaTime();
	if (time_ > popTime_) {
		Vector3 rect[4] = {
			(TransformNormal(rect_.leftTop , mat)) + translate,
			(TransformNormal(rect_.rightTop , mat)) + translate,
			(TransformNormal(rect_.leftBottom ,mat)) + translate,
			(TransformNormal(rect_.rightBottom, mat)) + translate
		};

		// 最初の四角形かどうかを判定
		if (uint32_t(nowTrailCount_) == 0) {
			/*auto& quad = quades.emplace_back();*/
			// 最初の四角形の場合、4つの頂点を追加
			verticesData_.push_back(VertexData({ Vector4(rect[0], 1.0f) }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
			verticesData_.push_back(VertexData({ Vector4(rect[1], 1.0f) }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
			verticesData_.push_back(VertexData({ Vector4(rect[2], 1.0f) }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
			verticesData_.push_back(VertexData({ Vector4(rect[3], 1.0f) }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

			// 最初の四角形のインデックスを設定
			indices_.push_back(0);
			indices_.push_back(1);
			indices_.push_back(2);
			indices_.push_back(1);
			indices_.push_back(3);
			indices_.push_back(2);

		} else {
			// それ以降の四角形は頂点を共有
			uint32_t baseIndex = uint32_t(nowTrailCount_) * 2; // 新しい頂点のインデックス
			//auto& quad = quades.emplace_back();

			verticesData_.push_back(VertexData({ Vector4(rect[0], 1.0f) }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
			verticesData_.push_back(VertexData({ Vector4(rect[1], 1.0f) }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

			indices_.push_back(baseIndex - 2); // 前回の rect[2]
			indices_.push_back(baseIndex - 1); // 前回の rect[3]
			indices_.push_back(baseIndex + 0); // 新しい rect[0]

			indices_.push_back(baseIndex - 1); // 前回の rect[3]
			indices_.push_back(baseIndex + 1); // 新しい rect[1]
			indices_.push_back(baseIndex + 0); // 新しい rect[0]
		}

		time_ = 0.0f;
		nowTrailCount_++;
	}
}

void Trail::ResetTrail() {
	verticesData_.clear();
	indices_.clear();

	vertexData_ = nullptr;
	indexData_ = nullptr;

	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	memcpy(vertexData_, verticesData_.data(), sizeof(VertexData) * verticesData_.size());
	memcpy(indexData_, indices_.data(), sizeof(uint32_t) * indices_.size());

	vertexBuffer_->Unmap(0, nullptr);
	indexBuffer_->Unmap(0, nullptr);

	nowTrailCount_ = 0;


	for (uint32_t oi = 0; oi < kMaxTrailNum_ * 4; ++oi) {
		vertexData_[oi] = VertexData();
	}

	for (uint32_t oi = 0; oi < kMaxTrailNum_ * 6; ++oi) {
		indexData_[oi] = uint32_t();
	}
}
