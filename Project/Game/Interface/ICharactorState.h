#pragma once
#include <string>
#include "Engine/Lib/IJsonConverter.h"

/// <summary>
/// キャラクターの状態基底クラス
/// </summary>
class ICharactorState {
public:

	struct Information : public IJsonConverter {
		std::string animationName;

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("animationName", animationName)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "animationName", animationName);
		}
	};

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

	Information information_;
};

