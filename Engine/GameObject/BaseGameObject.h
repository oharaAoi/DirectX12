#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/Assets/Animeter.h"

/// <summary>
/// 
/// </summary>
class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Finalize();
	virtual void Init();
	virtual void Update();
	virtual void Draw() const;
	void DrawSKinning() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetObject(const std::string& objName);

	void SetAnimater(const std::string& directoryPath, const std::string& objName);

	WorldTransform& GetTransform() { return transform_; }

protected:

	Model* model_;
	WorldTransform transform_;

	Animeter animeter_;
	Skeleton skeleton_;
	Skinning skinning_;
	bool isAnimation_ = false;
};