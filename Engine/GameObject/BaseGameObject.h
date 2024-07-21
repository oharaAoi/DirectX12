#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>

#include "Mesh.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"

enum class GameObjectType {
	Model_Type,
	Sphere_Type,
	Triangle_Type
};

class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュー座標</param>
	virtual void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection) = 0;

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	/// <param name="name">動かす対象の名前</param>
	virtual void ImGuiDraw(const std::string& name) = 0;

protected:



};