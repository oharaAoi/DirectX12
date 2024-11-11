#pragma once
#include <vector>
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Math/Easing.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/WorldObject/Rail.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GameScene;

/// <summary>
/// Railの中心点となる場所を編集するクラス
/// </summary>
class RailPointEditer {
public:

	/*struct RailData {
		Vector3 centerPosiont;
		float twist;

		RailData(const Vector3& newPoint, float newTwist) {
			centerPosiont = newPoint, twist = newTwist;
		}
	};*/

	struct RailPointData {
		Vector3 pos;
		Quaternion rotate;

		RailPointData(const Vector3& newPos, const Quaternion& newRotate) {
			pos = newPos, rotate = newRotate;
		}
	};

public:

	RailPointEditer();
	~RailPointEditer();

	void Init();
	void Update();
	void Draw(const Matrix4x4& vpMat) const;
	void DrawObject();

	std::vector<Vector3> createVector3Points(const Vector3& start, const Vector3& direction, int count);

	void Load();

	void InitLoad();

	void Save();

#ifdef _DEBUG
	void EditRail();
	void Debug_Gui();
#endif

	std::vector<Vector3> GetRailPoints();
	std::vector<Vector3> GetPoints();
	std::vector<float> GetPointsRotate();

	std::vector<Vector3> GetRailBasePoints();

	std::vector<float> GetRailRotateZ();
	
	const Vector3 GetRailPos(size_t index) const;
	const Quaternion GetRailRotate(size_t index) const;

	const Vector3 GetPoints(size_t index) const;
	const Quaternion GetPointsRotate(size_t index) const;

	const size_t GetRailNum() const { return railPoints.size(); }

	void SetIsAdd(bool isAdd) { isAdd_ = isAdd; }

	void SetGameScene(GameScene* gameScene) { pGameScene_ = gameScene; }

	uint32_t GetPointsSize() const { return points_.size(); }

private:

	GameScene* pGameScene_;

	std::vector<RailPointData> railPoints;
	std::vector<Vector3> railIndexPoints_;
	std::vector<float> railIndexPointsRotateZ_;

	std::vector<Vector3> points_;
	std::vector<Quaternion> pointsRotateZ_;

	std::vector<std::unique_ptr<Rail>> rails_;

	const std::string directoryPath_ = "Game/Resources/GameData/Rail/";
	const std::string railFile_ = "railData";


	// 新しく点を追加する際のパラメータ
	Vector3 newPoint_;
	Quaternion newRotate_;
	std::unique_ptr<BaseGameObject> newRail_;
	float newTwist_;

	int easeType_ = (int)EasingType::Out::Cubic;

	bool isAdd_;

	char selectPointIndex_[64];
	std::string selectIndex_ = "0";
};

