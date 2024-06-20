#pragma once
#include "Mesh.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Sphere {
public:

	Sphere();
	~Sphere();

	void Init(ID3D12Device* device, const uint32_t& division);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void ImGuiDraw(const std::string& name);

	void SetMaterials(const float& roughness, const float& metallic);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	std::unique_ptr<TransformationMatrix> transformation_;

	// 頂点数
	uint32_t vertexCount_;

};