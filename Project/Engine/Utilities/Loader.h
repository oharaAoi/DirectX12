#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
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
/// mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <param name="scale"></param>
void LoadMtl(const std::string& directoryPath, const std::string& fileName, Vector3& scale);

//================================================================================================//
//
// 拡張子系
//
//================================================================================================//

const char* GetFileExtension(const char* filename);
std::string RemoveExtension(const std::string& filename);