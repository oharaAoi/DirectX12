#pragma once
#include <list>
#include <functional>
#include <string>

/// <summary>
/// GameObjectをまとめたwindow
/// </summary>
class GameObjectWindow {
public:

	GameObjectWindow();
	~GameObjectWindow();

	void Init();

	void AddFunction(std::function<void()> func, const std::string& label);

#ifdef _DEBUG
	void Edit();
#endif // _DEBUG
private:

	using Pair = std::pair<std::string, std::function<void()>>;

	std::list<Pair> functionList;

};

