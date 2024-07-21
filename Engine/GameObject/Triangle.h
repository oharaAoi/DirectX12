#pragma once
#include "BaseGameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Triangle
	: public BaseGameObject {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex);

	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection) override;

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	/// <param name="name">: 動かす対象の名前</param>
	void ImGuiDraw(const std::string& name) override;

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	
};
