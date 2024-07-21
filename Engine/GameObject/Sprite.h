#pragma once
#include "Mesh.h"
#include "Material.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Sprite {
public:

	struct TextureTransformData {
		Matrix4x4 wvp;
	};

public:

	Sprite();
	~Sprite();

	void Init(ID3D12Device* device, const Mesh::RectVetices& vertex);

	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	ComPtr<ID3D12Resource> transformBuffer_;

	kTransform transform_;
	kTransform uvTransform_;
	TextureTransformData* transformData_;

};