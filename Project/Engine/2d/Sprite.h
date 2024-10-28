#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Assets/Mesh.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"

class Render;

class Sprite {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color;
		Matrix4x4 uvTransform;
		Vector2 uvDrawRange;		// 0~1の範囲で指定
		float padding[2];
	};

	struct TextureTransformData {
		Matrix4x4 wvp;
	};

public:

	Sprite();
	~Sprite();

	void Init(ID3D12Device* device, const std::string& fileName);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="rectRange">: 描画する範囲</param>
	/// <param name="leftTop">: 左上座標</param>
	void Update();
	void Draw(bool isBackGround = false);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void PostDraw(ID3D12GraphicsCommandList* commandList) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	/// <summary>
	/// Textureをセットする
	/// </summary>
	/// <param name="fileName">: セットするTexture名</param>
	void SetTexture(const std::string& fileName);

	/// <summary>
	/// Textureの中心位置を変える(Screen座標系)
	/// </summary>
	/// <param name="centerPos">: position</param>
	void SetTextureCenterPos(const Vector2& centerPos);

	/// <summary>
	/// Textureのサイズを再設計する
	/// </summary>
	/// <param name="size"></param>
	void SetTextureSize(const Vector2& size);

	// 描画する範囲の設定
	void SetRectRange(const Vector2& rectRange) { rectRange_ = rectRange; };
	// 描画する範囲の設定
	void SetLeftTop(const Vector2& leftTop) { leftTop_ = leftTop; }
	// Pivotの位置を変更する
	void SetAnchorPoint(const Vector2& point) { anchorPoint_ = point; }

	void SetCenterPos(const Vector2 pos) { transform_.translate.x = pos.x, transform_.translate.y = pos.y; }
	void SetScale(const Vector2 scale) { transform_.scale.x = scale.x, transform_.scale.y = scale.y, transform_.scale.z = 1.0f; }
	void SetRotate(float rotate) { transform_.rotate.z = rotate; }

	void SetColor(const Vector4& color) { materialData_->color = color; };
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }

	/// <summary>
	/// UVを直接する
	/// </summary>
	/// <param name="range"></param>
	void SetUvDrawRange(const Vector2& range) { materialData_->uvDrawRange = range; }

	const Vector2 GetCenterPos() const { return Vector2{ transform_.translate.x, transform_.translate.y}; }
	const Vector2 GetScale() const { return Vector2(transform_.scale.x, transform_.scale.y); }
	const float GetRotate() const { return transform_.rotate.z; }
	const Vector2 GetTextureSize() const { return textureSize_; }
	const bool GetIsFlipX() const { return isFlipX_; }
	const bool GetIsFlipY() const { return isFlipY_; }
	const Vector2 GetUvRangeDraw() const { return materialData_->uvDrawRange; }

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> transformBuffer_;

	// view
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	// データ
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;
	TextureTransformData* transformData_;

	// Transform情報
	kTransform transform_;
	kTransform uvTransform_;

	std::string textureName_;

	// 描画する範囲
	Vector2 rectRange_ = { 0.0f, 0.0f };
	// 左上座標
	Vector2 leftTop_ = { 0.0f, 0.0f };
	Vector2 centerPos_; 
	
	Vector2 anchorPoint_;

	bool isFlipX_ = false;	// 左右フリップ
	bool isFlipY_ = false;	// 上下フリップ

	// Textureのサイズ
	Vector2 textureSize_;
};