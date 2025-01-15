#pragma once
#include <vector>
#include <string>
#include <unordered_map>
//#include <Lib/tiny_gltf.h> // Assimpの場合使わない
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Rigging/SkinCluster.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Assets/Animation/AnimationStructures.h"
#include "Engine/Math/MathStructures.h"
#include <memory>

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
		QuaternionSRT transform;
		Matrix4x4 localMatrix;				 // NodeのLocalMatrix
		std::string name;					 // Nodeの名前
		std::vector<Node> children;			 // 子供のNode
		NodeAnimationData animationsData;	 // ノードに関するアニメーション
	};

	struct ModelMaterialData {
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		int32_t enableLighting = 1;
		Matrix4x4 uvTransform = Matrix4x4::MakeUnit();
		float shininess = 1.0f;
		std::string textureFilePath = "white.png"; // 使用するtextureのパス
	};

public:

	Model();
	~Model();

	void Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName);
	
	void Draw(ID3D12GraphicsCommandList* commandList,
			  const WorldTransform* worldTransform, const ViewProjection* viewprojection,
			  const std::vector<std::unique_ptr<Material>>& materials);
	
	void Draw(ID3D12GraphicsCommandList* commandList,
			  const WorldTransform* worldTransform, const ViewProjection* viewprojection,
			  const D3D12_VERTEX_BUFFER_VIEW& vbv, const std::vector<std::unique_ptr<Material>>& materials);

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
	//void LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

public:

	const std::string& GetRootNodeName() const { return rootNode_.name; }

	Node& GetNode() { return rootNode_; }

	const std::map<std::string, JointWeightData>& GetSkinClustersData(uint32_t index) { return skinClusterArray_[index]->GetSkinClustersData(); }

	std::unordered_map<std::string, Model::ModelMaterialData>& GetMaterialData() { return materialData_; }

	Mesh* GetMesh(const uint32_t& index);
	size_t GetMeshsNum() const { return meshArray_.size(); }

	const size_t GetMaterialsSize() const { return materialData_.size(); }

private:

	// 頂点バッファやインデックスバッファを持つ
	std::vector<std::unique_ptr<Mesh>> meshArray_;
	// materialの情報
	std::unordered_map<std::string, Model::ModelMaterialData> materialData_;
	// skinningのデータ
	std::vector<std::unique_ptr<SkinCluster>> skinClusterArray_;
	// ノード
	Node rootNode_;
};