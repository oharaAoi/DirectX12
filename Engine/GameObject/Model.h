#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <Lib/tiny_gltf.h> // Assimpの場合使わない
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/AnimationUtils.h"

class Model {
public:

	struct NodeAnimationData {
		std::vector<NodeAnimation> animations;
		float tickPerSecond;	// 一秒間に何回の処理が行われるか
		float duration;			// tickPerSecondの持続時間
		float animationTime;	// animationをする時間
	};

	struct Node {
		QuaternionTransform transform;
		Matrix4x4 localMatrix;				 // NodeのLocalMatrix
		std::string name;					 // Nodeの名前
		std::vector<Node> children;			 // 子供のNode
		NodeAnimationData animationsData; // ノードに関するアニメーション
	};

public:

	Model();
	~Model();

	void Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName);

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewprojection);

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	/// <param name="name">: 動かす対象の名前</param>
	void ImGuiDraw(const std::string& name);

	/// <summary>
	/// assimpでのNode解析
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	Node ReadNode(aiNode* node, const aiScene* scene);

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

	/// <summary>
	/// assimpを使用してモデルファイルをを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <param name="device"></param>
	void LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

	void LoadAnimation(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// アニメーションの更新を行う
	/// </summary>
	/// <returns></returns>
	void AnimationUpdate();

public:

	bool GetHasTexture() const { return hasTexture_; }

	std::string GetRootNodeName() { return rootNode_.name; }

	Node& GetNode() { return rootNode_; }

private:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// テクスチャの情報を持っている
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;
	// ノード
	Node rootNode_;

	// モデルにtextureがあるか
	bool hasTexture_;

	//////////////////////////////////////////////////////////

	// アニメーションの時間
	float currentAnimationTime_ = 0;
	bool isBack = false;
	kTransform localTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f },{0.0f, 0.0f, 0.0f} };
};