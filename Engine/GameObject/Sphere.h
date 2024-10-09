#pragma once
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Manager/TextureManager.h"

class Sphere {
public:

	Sphere();
	~Sphere();

	void Init(ID3D12Device* device, const uint32_t& division);

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform* worldTransform, const ViewProjection* viewProjection);

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	/// <param name="name">: 動かす対象の名前</param>
	void ImGuiDraw(const std::string& name);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;

	// 頂点数
	uint32_t vertexCount_;

};