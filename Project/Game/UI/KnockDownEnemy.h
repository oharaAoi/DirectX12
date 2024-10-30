#pragma once
#include <string>
#include <vector>
#include "Engine/Engine.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Math/MyMath.h"

class KnockDownEnemy : public BaseGameObject {
public:

	KnockDownEnemy();
	~KnockDownEnemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void CalculateionScore();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	static void SetObj(const std::string& name, uint32_t score);

	void SetWorldPos(const Vector3& worldPos) { worldPos_ = worldPos; }
	void SetScreenPos(const Vector2& screenPos) { screenPos_ = screenPos; }

	const Vector3 GetWorldPos() const { return worldPos_; }
	const Vector2 GetScreenPos() const { return screenPos_; }

private:

	static std::string knockObjName_;
	static bool isChange_;
	static uint32_t knockScore_;

	Vector2 screenPos_;
	Vector3 worldPos_;

	float rotateCount_;
	float rotateTime_;
	float rotateAngle_;

	std::vector<std::unique_ptr<Sprite>> knockScoreArray_;
	Vector2 numberSpriteSize_;
	Vector2 originPos_;
};

