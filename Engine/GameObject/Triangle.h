#pragma once
#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Triangle {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex);

	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	
};
