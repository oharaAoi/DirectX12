#pragma once
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "Engine/Engine.h"
#include "Engine/Render.h"
#include "Engine/Input/Input.h"
#include "Engine/Math/Vector3.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyMath.h"

using json = nlohmann::json;

/// <summary>
/// Bossの手の動きを制御するEdite
/// </summary>
class BossAttackEditer {
public:

	BossAttackEditer();
	~BossAttackEditer();

	void Init();
	void Update();
	void Draw() const;

#ifdef _DEBUG
	void AddPoint();

	void DeletePoint(const Matrix4x4& vpvpMat);
#endif 

	std::vector<Vector3>& GetMovePoints() { return movePoints_; }

private:

	uint32_t segmentCount_;
	// 実際に動く座標
	std::vector<Vector3> movePoints_;
	// 制御点
	std::vector<Vector3> controlPoint_;
	std::vector<std::unique_ptr<BaseGameObject>> controlPointObjects_;
	// 新しく生成する座標
	Vector3 newPoint_;
	uint32_t selectIndex_ = 0;
};

