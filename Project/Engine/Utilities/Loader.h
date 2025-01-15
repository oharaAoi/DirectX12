#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/Assets/Rigging/SkinCluster.h"
#include "Engine/Assets/Animation/AnimationClip.h"
#include "Engine/System/Manager/TextureManager.h"

//================================================================================================//
//
// ファイルの読み込み系
//
//================================================================================================//

/// <summary>
/// meshの情報を読み込む
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <param name="device">デバイス</param>
/// <returns></returns>
std::vector<std::unique_ptr<Mesh>> LoadMesh(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

/// <summary>
/// materialの情報を読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <param name="device"></param>
/// <returns></returns>
std::unordered_map<std::string, std::unique_ptr<Material>> LoadMaterialData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device);

/// <summary>
/// materialを構成する情報の読み込み
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
std::unordered_map<std::string, Model::ModelMaterialData> LoadMaterialData(const std::string& directoryPath, const std::string& fileName);

/// <summary>
/// mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <param name="scale"></param>
void LoadMtl(const std::string& directoryPath, const std::string& fileName, Vector3& scale);

/// <summary>
/// 
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
Model::Node LoadNode(const std::string& directoryPath, const std::string& fileName);

Model::Node ReadNode(aiNode* node, const aiScene* scene);

/// <summary>
/// Animationを読み込む
/// </summary>
/// <param name="directoryPath">: directoryPath</param>
/// <param name="animationFile">: fileName</param>
/// <returns> Animationデータ </returns>
std::unordered_map<std::string, Animation> LoadAnimation(const std::string directoryPath, const std::string& animationFile);

/// <summary>
/// skinCluster用のデータを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
std::vector<std::unique_ptr<SkinCluster>> LoadSkinCluster(const std::string& directoryPath, const std::string& fileName);

//================================================================================================//
//
// 拡張子系
//
//================================================================================================//

const char* GetFileExtension(const char* filename);
std::string RemoveExtension(const std::string& filename);