#pragma once
#include <string>

/// <summary>
/// Objectの状態を表す基底クラス
/// </summary>
class BaseObjectState {
public:

	BaseObjectState() {};
	virtual ~BaseObjectState() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;

	const std::string GetStateName() const { return stateName_; }


#ifdef _DEBUG
	virtual void Debug_Gui() = 0;
#endif // _DEBUG

protected:

	std::string stateName_;	// 状態の名前

};

