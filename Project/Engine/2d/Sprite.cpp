#include "Sprite.h"
#include "Render.h"

Sprite::Sprite() {}
Sprite::~Sprite() {
	vertexData_ = nullptr;
	indexData_ = nullptr;
	materialData_ = nullptr;
	transformData_ = nullptr;
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transformBuffer_.Reset();
}

void Sprite::Init(ID3D12Device* device, const std::string& fileName) {
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	textureName_ = fileName;
	rectRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };
	anchorPoint_ = { 0.5f, 0.5f };

	// ----------------------------------------------------------------------------------
	vertexBuffer_ = CreateBufferResource(device, sizeof(TextureMesh) * 4);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(TextureMesh) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(TextureMesh);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	Vector3 pivotOffset = {
		textureSize_.x * anchorPoint_.x,
		textureSize_.y * anchorPoint_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;		// 左下
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[1].pos = rect.leftTop;			// 左上
	vertexData_[1].texcoord = { 0.0f, 0.0f };	
	vertexData_[2].pos = rect.rightBottom;		// 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;			// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

	// ----------------------------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;

	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
	// ----------------------------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->uvDrawRange = { 1.0f, 1.0f };
	
	// ----------------------------------------------------------------------------------
	transformBuffer_ = CreateBufferResource(device, sizeof(TextureTransformData));
	transformBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };
	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	transformData_->wvp = Matrix4x4(
		MakeAffineMatrix(transform_)
		* MakeIdentity4x4()
		* MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::Update() {
	materialData_->uvTransform = MakeAffineMatrix(uvTransform_);
	
	// -------------------------------------------------
	// ↓ 頂点の変更
	// -------------------------------------------------
	//Vector3 pivotOffset = {
	//	textureSize_.x * anchorPoint_.x,
	//	textureSize_.y * anchorPoint_.y,
	//	0.0f
	//};

	//// スプライトのサイズを基に頂点を設定
	//Mesh::RectVetices rect = {
	//	{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
	//	{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
	//	{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
	//	{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	//};

	///*Mesh::RectVetices rect = {
	//	{-(textureSize_.x * 0.5f) + pivotOffset.x, -(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
	//	{+(textureSize_.x * 0.5f) + pivotOffset.x, -(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
	//	{-(textureSize_.x * 0.5f) + pivotOffset.x, +(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
	//	{+(textureSize_.x * 0.5f) + pivotOffset.x, +(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
	//};*/

	//vertexData_[0].pos = rect.leftBottom;		// 左下
	//vertexData_[0].texcoord = { 0.0f, 1.0f };
	//vertexData_[1].pos = rect.leftTop;			// 左上
	//vertexData_[1].texcoord = { 0.0f, 0.0f };
	//vertexData_[2].pos = rect.rightBottom;		// 右下
	//vertexData_[2].texcoord = { 1.0f, 1.0f };
	//vertexData_[3].pos = rect.rightTop;			// 右上
	//vertexData_[3].texcoord = { 1.0f, 0.0f };

	// -------------------------------------------------
	// ↓ UVの変更
	// -------------------------------------------------
	materialData_->uvTransform.m[0][0] = rectRange_.x / textureSize_.x;	// Xスケーリング
	materialData_->uvTransform.m[1][1] = rectRange_.y / textureSize_.y;	// Yスケーリング
	materialData_->uvTransform.m[3][0] = leftTop_.x / textureSize_.x;	// Xオフセット
	materialData_->uvTransform.m[3][1] = leftTop_.y / textureSize_.y;	// Yオフセット
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画前処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::Draw(bool isBackGround) {
	Vector2 pivotOffset = {
		(anchorPoint_.x - 0.5f) * textureSize_.x,  // ピボットオフセット（中心からのオフセット）
		(anchorPoint_.y - 0.5f) * textureSize_.y   // ピボットオフセット（中心からのオフセット）
	};

	Matrix4x4 projection = Render::GetProjection2D();
	if (isBackGround) {
		transform_.translate.z = Render::GetFarClip();
	}
	// アフィン変換行列の作成
	Matrix4x4 affineMatrix = MakeAffineMatrix(transform_);
	// テクスチャ位置を保持するための補正行列
	Matrix4x4 correctionTranslation = MakeTranslateMatrix({ pivotOffset.x, pivotOffset.y, 0.0f });

	// 最終的なスプライトの変換行列
	transformData_->wvp = Matrix4x4(
		affineMatrix *  // ピボットによる変位を元に戻す
		correctionTranslation *
		projection
	);

	Render::DrawSprite(this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　実際にDrawCallを呼び出す
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::PostDraw(ID3D12GraphicsCommandList* commandList) const {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUVirtualAddress());
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName_, 2);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Textureを再設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::SetTexture(const std::string& fileName) {
	textureName_ = fileName;
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	rectRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };

	Vector3 pivotOffset = {
		textureSize_.x * anchorPoint_.x,
		textureSize_.y * anchorPoint_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Textureの中心位置を変更する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::SetTextureCenterPos(const Vector2& centerPos) {
	transform_.translate.x = centerPos.x;
	transform_.translate.y = centerPos.y;
	transform_.translate.z = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Textureのサイズを再設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::SetTextureSize(const Vector2& size) {
	Vector3 pivotOffset = {
		size.x * anchorPoint_.x,
		size.y * anchorPoint_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{size.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, size.y - pivotOffset.y, 0.0f , 1.0f},
		{size.x - pivotOffset.x, size.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Sprite::Debug_Gui() {
	if (ImGui::TreeNode("transform")) {
		ImGui::DragFloat3("translation", &transform_.translate.x, 0.1f);
		ImGui::DragFloat2("scale", &transform_.scale.x, 0.01f);
		ImGui::SliderAngle("rotation", &transform_.rotate.z);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("uv")) {
		ImGui::DragFloat2("uvTranslation", &uvTransform_.translate.x, 0.01f);
		ImGui::DragFloat2("uvScale", &uvTransform_.scale.x, 0.01f);
		ImGui::SliderAngle("uvRotation", &uvTransform_.rotate.z);
		ImGui::TreePop();
	}
	ImGui::DragFloat2("anchorPoint", &anchorPoint_.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("textureSize", &textureSize_.x, 1.0f);
	ImGui::DragFloat2("rectRange", &rectRange_.x, 1.0f);
	ImGui::DragFloat2("leftTop", &leftTop_.x, 1.0f);
	ImGui::DragFloat2("uvDrawRange", &materialData_->uvDrawRange.x, 0.01f);


}
#endif


//float left = 0.0f - anchorPoint_.x;
	//float right = 1.0f - anchorPoint_.x;
	//float top = 0.0f - anchorPoint_.y;
	//float bottom = 1.0f - anchorPoint_.y;

	//// 左右反転
	//if (isFlipX_) {
	//	left = -left;
	//	right = -right;
	//}

	//// 上下反転
	//if (isFlipY_) {
	//	top = -top;
	//	bottom = -bottom;
	//}

	//vertexData_[0].pos = { left, bottom, 0.0f, 1.0f };	// 左下
	//vertexData_[1].pos = { left, top, 0.0f, 1.0f };		// 左上
	//vertexData_[2].pos = { right, bottom, 0.0f, 1.0f }; // 右下
	//vertexData_[3].pos = { right, top, 0.0f, 1.0f };	// 右上
