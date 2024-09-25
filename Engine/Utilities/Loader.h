#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>

#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Manager/TextureManager.h"

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

