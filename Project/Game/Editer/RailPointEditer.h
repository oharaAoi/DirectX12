#pragma once
#include <vector>
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/MyMath.h"
#include "Game/WorldObject/Rail.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// <summary>
/// Railの中心点となる場所を編集するクラス
/// </summary>
class RailPointEditer {
public:

	struct RailData {
		Vector3 centerPosiont;
		float twist;

		RailData(const Vector3& newPoint, float newTwist) {
			centerPosiont = newPoint, twist = newTwist;
		}
	};

public:

	RailPointEditer();
	~RailPointEditer();

	void Init();
	void Update();
	void Draw(const Matrix4x4& vpMat) const;
	void DrawObject();

	void Save();

#ifdef _DEBUG
	void EditRail();
	void Debug_Gui();
#endif

	std::vector<Vector3> GetRailPoints();

	std::vector<Vector3> GetRailBasePoints();

	const size_t GetRailNum() const { return railPoints.size(); }

private:

	std::vector<RailData> railPoints;
	std::vector<Vector3> railIndexPoints_;
	std::vector<std::unique_ptr<Rail>> rails_;

	// 新しく点を追加する際のパラメータ
	Vector3 newPoint_;
	float newTwist_;

	char selectPointIndex_[64];
	std::string selectIndex_ = "0";
};

