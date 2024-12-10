#pragma once
#include <string>

/// <summary>
/// キャラクターの状態基底クラス
/// </summary>
class ICharactorState {
public:

	ICharactorState();
	virtual ~ICharactorState() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;

	const std::string GetStateName() const { return stateName_; }

#ifdef _DEBUG
	virtual void Debug_Gui() = 0;
#endif // _DEBUG

protected:

	std::string stateName_;	// 状態の名前


};

