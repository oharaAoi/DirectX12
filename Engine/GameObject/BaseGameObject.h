#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"

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

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetObject(const std::string& objName);


protected:

	std::unique_ptr<Model> model_;
	WorldTransform transform_;

};