#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"

class TutorialUI {
public:

	TutorialUI();
	~TutorialUI();

	void Init();
	void Update(const Vector3& playerPos);
	void Draw() const;

	void AdaptItem();

	void PopUp();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	static const uint32_t kGuideMax_ = 4;

	std::unique_ptr<BaseGameObject> gaide_[kGuideMax_];

	float popUpTime_ = 0.0f;
	float popUpTimeLimit_ = 0.5f;

	uint32_t preNearIndex = 0;
	uint32_t nearIndex_ = 0;

	bool isChange_ = false;

};

