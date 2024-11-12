#pragma once
#include <memory>
#include <list>
#include <map>
#include "Game/GameObject/Enemy.h"
#include "Engine/Math/MyRandom.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();

	void Init();
	void Update(uint32_t eyeIndex);
	void Draw() const;

	void AddList(const Vector3& popPos);

	void Load(const std::string& fileName);

	void LoadAllFile();

	void Pop(uint32_t index);

#ifdef _DEBUG
	void Debug_Gui();

	void Edit();

	void Edit_Pop();
	void Debug_Pop(uint32_t index);

	void Save();
#endif

	std::list<std::unique_ptr<Enemy>>& GetList() { return enemyList_; }

	void SetPlayerPos(const Vector3& pos) { playerPos_ = pos; }
	void SetPlayerForward(const Vector3& forward) { playerForward_ = forward; }

private:

	struct NewEnmeyData {
		Vector3 translate_;
		Vector3 velocity_;
		EnemyType enemyType_;
		
		NewEnmeyData(const Vector3& translate, const Vector3& velocity, const EnemyType& type) {
			translate_ = translate, velocity_ = velocity, enemyType_ = type;
		};
	};

private:

	// enemyのリスト
	std::list<std::unique_ptr<Enemy>> enemyList_;

	// enemyの出現をまとめたファイル
	std::map<uint32_t, std::vector<NewEnmeyData>> popEnemyData_;

	std::vector<uint32_t> popData_;

	// --- playerの情報 ----------------------------- //
	// playerの座標
	Vector3 playerPos_;
	// playerの前方向のベクトル
	Vector3 playerForward_;
	
	// --- Parameter ------------------------------- //
	float popTime_;

	bool isPop_;

	// -------------------------------------------------
	// ↓ Editerに関する変数
	// -------------------------------------------------

	const std::string directoryPath_ = "Game/Resources/GameData/EnemyData/";
	std::string enemyDataFile_;
	uint32_t popIndex_;

	std::unique_ptr<Enemy> newEnmey_;

	Vector3 newTranslate_;
	Vector3 newVelocity_;
	EnemyType newEnemyType_;

	std::list<std::unique_ptr<Enemy>> popEnemies_;

	uint32_t debugPopIndex_;

};

