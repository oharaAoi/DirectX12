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

class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection);

	virtual void Draw(ID3D12GraphicsCommandList* commandList);

protected:



};