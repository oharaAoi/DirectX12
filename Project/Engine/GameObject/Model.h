#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <unordered_map>
//#include <Lib/tiny_gltf.h> // Assimpの場合使わない
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Skinning.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/MeshManager.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Utilities/AnimationUtils.h"
#include "Engine/Lib/GameTimer.h"

class Material;
class PBRMaterial;

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

	struct ModelMaterialData {
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		int32_t enableLighting = 1;
		Matrix4x4 uvTransform = MakeIdentity4x4();
		float shininess = 1.0f;
		std::string textureFilePath; // 使用するtextureのパス
	};

public:

	Model();
	~Model();

	void Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform* worldTransform, const ViewProjection* viewprojection, const std::vector<std::unique_ptr<Material>>& materials);
	void DrawSkinning(ID3D12GraphicsCommandList* commandList, const Skinning* skinning, const WorldTransform* worldTransform, const ViewProjection* viewprojection, const std::vector<std::unique_ptr<Material>>& materials);
	void DrawSkinnings(ID3D12GraphicsCommandList* commandList, const std::vector<std::unique_ptr<Skinning>>& skinnig, const WorldTransform* worldTransform, const ViewProjection* viewprojection, const std::vector<std::unique_ptr<Material>>& materials);


#ifdef _DEBUG
	void Debug_Gui(const std::string& name);
#endif

	/// <summary>
	/// assimpでのNode解析
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	Node ReadNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// assimpを使用してモデルファイルをを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <param name="device"></param>
	void LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

public:

	bool GetHasTexture() const { return hasTexture_; }

	const std::string& GetRootNodeName() const { return rootNode_.name; }

	Node& GetNode() { return rootNode_; }

	std::map<std::string, Skinning::JointWeightData>& GetSkinClusterData() { return data_; }
	std::map<std::string, Skinning::JointWeightData>& GetSkinClustersData(uint32_t index) { return skinClusterData_[index]; }

	Mesh* GetMesh(const uint32_t& index);
	size_t GetMeshsNum() const { return meshArray_.size(); }

	const ModelMaterialData GetMaterialData(const std::string& name)const {return materialData_.at(name);}
	const size_t GetMaterialsSize() const { return materialData_.size(); }

private:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::shared_ptr<Mesh>> meshArray_;
	std::unordered_map<std::string, ModelMaterialData> materialData_;
	// テクスチャの情報を持っている
	//std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;

	std::vector<ModelMaterialData> materialsData_;

	std::vector<std::map<std::string, Skinning::JointWeightData>> skinClusterData_;
	std::map<std::string, Skinning::JointWeightData> data_;
	// ノード
	Node rootNode_;

	// モデルにtextureがあるか
	bool hasTexture_;

	//////////////////////////////////////////////////////////

	// アニメーションの時間
	float currentAnimationTime_ = 0;
};