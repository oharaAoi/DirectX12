#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"

class ObjectAxis {
public:

	ObjectAxis();
	~ObjectAxis();

	void Finalize();
	void Init();
	void Update(const Matrix4x4& parentWorldMat);
	void Draw() const;

private:

	Model* model_;
	std::vector<std::unique_ptr<Material>> materials;

	std::unique_ptr<WorldTransform> transform_;

};

