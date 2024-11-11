#pragma once
#include <map>
#include <memory>
#include <string>
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Worldのオブジェクトをまとめたクラス
/// </summary>
class WorldObjects {
public:

	WorldObjects();
	~WorldObjects();

	void Init();
	void Update();
	void Draw() const;

private:

	std::map < std::string, std::unique_ptr<BaseGameObject>> objectMap_;

};

