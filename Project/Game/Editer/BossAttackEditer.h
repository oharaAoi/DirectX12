#pragma once
#include <vector>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>
#include "Engine/Engine.h"
#include "Engine/Render.h"
#include "Engine/Input/Input.h"
#include "Engine/Math/Vector3.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Math/Easing.h"

using json = nlohmann::json;

/// <summary>
/// Bossの手の動きを制御するEdite
/// </summary>
class BossAttackEditer {
public:

	struct HandMoveData {
		// SRT
		Vector3 scale;
		Quaternion rotate;
		Vector3 translate;
		// Easing関数
		int easeType;
		// その間の移動を何秒で行うか
		float moveTimeLimit;
	};

public:

	BossAttackEditer();
	~BossAttackEditer();

	void Init();
	void Update();
	void Draw() const;

	void LoadAllFile(const std::string& directoryPath);
	void LoadFile(const std::string& directoryPath, const std::string& fileName);

#ifdef _DEBUG
	void AddPoint();

	void AddPoint(const Vector3& scale, const Quaternion& rotate, const Vector3& translate, int easeType, float moveTimeLimit);

	void DeletePoint(const Matrix4x4& vpvpMat);

	void EditHandMoves();

	void Debug_Gui(const std::string& directoryPath);

	void SaveAttack(const std::string& directoryPath);

	void SelectAttack();

	const std::string GetSelectFileName() const { return selectFileName_; }
#endif 

	std::vector<Vector3>& GetMovePoints() { return movePoints_; }

	std::vector<HandMoveData>& GetHandMoves() { return handMoves_; }

	void SetHandMoves(const std::string& fileName);

private:

	uint32_t segmentCount_;
	// 実際に動く座標
	std::vector<Vector3> movePoints_;
	// 制御点
	std::vector<Vector3> controlPoint_;
	std::vector<std::unique_ptr<BaseGameObject>> controlPointObjects_;

	// 実際に手が動く情報が入った変数
	std::vector<HandMoveData> handMoves_;
	// 攻撃の情報をまとめた変数
	std::unordered_map<std::string, std::vector<HandMoveData>> attackMoveMap_;
	std::vector<std::string> attackFileNames_;

	// 新しく生成する座標
	Vector3 newPoint_;

	std::string fileName_;

#ifdef _DEBUG
	uint32_t selectIndex_ = 0;
	std::string selectFileName_;
#endif 
};

