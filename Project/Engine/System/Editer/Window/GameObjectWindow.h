#pragma once
#include <vector>
#include <functional>
#include <string>
#include "Engine/Components/Attribute/AttributeGui.h"

/// <summary>
/// GameObjectをまとめたwindow
/// </summary>
class GameObjectWindow {
public:

	GameObjectWindow();
	~GameObjectWindow();

	void Init();

	void AddFunction(AttributeGui* attribute, const std::string& label);

#ifdef _DEBUG
	void Edit();
#endif // _DEBUG
private:

	using Pair = std::pair<std::string, AttributeGui*>;

	std::vector<Pair> attributeArray_;

};

