#pragma once
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Assets/TransformationMatrix.h"
#include "Engine/Manager/TextureManager.h"

class Engine;

class Triangle {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex, const std::string& textureName);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, ViewProjection* viewProjection) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	WorldTransform* GetTransform() { return worldTransform_.get(); }

	void SetColor(const Vector4& color) { material_->SetColor(color); };
	void SetTexture(const std::string& textureName) { textureName_ = textureName; }

	void SetVertices(const Mesh::Vertices& vertex);

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	std::unique_ptr<WorldTransform> worldTransform_;

	std::string textureName_;
};
