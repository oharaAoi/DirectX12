#pragma once
#include "BaseGameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Sphere {
public:

	Sphere();
	~Sphere();

	void Init(ID3D12Device* device, const uint32_t& division);

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection);

	void ImGuiDraw(const std::string& name);

	void SetMaterials(const float& roughness, const float& metallic);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;

	// 頂点数
	uint32_t vertexCount_;

};