#pragma once
#include <vector>
#include <string>
#include "Engine/Math/Vector3.h"

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

#ifdef _DEBUG
	void EditRail();
	void Debug_Gui();
#endif

private:

	std::vector<RailData> railPoints;

	// 新しく点を追加する際のパラメータ
	Vector3 newPoint_;
	float newTwist_;

	char selectPointIndex_[64];
	std::string selectIndex_ = "0";
};

