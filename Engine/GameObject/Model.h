#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Model {
public:

	struct Node {
		Matrix4x4 localMatrix;		// NodeのLocalMatrix
		std::string name;			// Nodeの名前
		std::vector<Node> children;	// 子供のNode
	};

public:

	Model();
	~Model();

	void Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName);

	void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void ImGuiDraw(const std::string& name);

	void SetMaterials(const float& roughness, const float& metallic);

	/// <summary>
	/// assimpでのNode解析
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	Node ReadNode(aiNode* node);

public:

	/// <summary>
	/// meshの情報を読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	std::vector<std::unique_ptr<Mesh>> LoadVertexData(const std::string& filePath, ID3D12Device* device);

	/// <summary>
	/// materialの情報を読み込む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <param name="device"></param>
	/// <returns></returns>
	std::unordered_map<std::string, std::unique_ptr<Material>> LoadMaterialData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

	void LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

public:

	bool GetHasTexture() const { return hasTexture_; }

private:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;
	// 
	std::unique_ptr<TransformationMatrix> transformation_;

	// モデルにtextureがあるか
	bool hasTexture_;

};