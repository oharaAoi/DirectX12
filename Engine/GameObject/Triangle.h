#pragma once
#include "Mesh.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Triangle {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	std::unique_ptr<TransformationMatrix> transformation_;

};
