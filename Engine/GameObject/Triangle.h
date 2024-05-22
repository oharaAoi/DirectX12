#pragma once
#include "Mesh.h"

class Triangle {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	std::unique_ptr<Mesh> mesh_;

};