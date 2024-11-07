#include "EnemyManager.h"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Init() {
	enemyList_.clear();
	isPop_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Update() {
	if (isPop_) {
		popTime_ += GameTimer::DeltaTime();

		if (popTime_ > 6.0f) {
			AddList(playerPos_);
			popTime_ = 0.0f;
		}
	}

	// 生存確認
	enemyList_.remove_if([](auto& enemy) {return !enemy->GetIsAlive(); });

	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemyList_.begin(); itr != enemyList_.end();) {
		(*itr)->Update();
		++itr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Draw() const {
	for (std::list<std::unique_ptr<Enemy>>::const_iterator itr = enemyList_.begin(); itr != enemyList_.end();) {
		(*itr)->Draw();
		++itr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Enemyの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::AddList(const Vector3& popPos) {
	Vector3 pos = popPos + (playerForward_ * 80.0f);
	
	Vector3 randomPos = Vector3(
		RandomFloat(-5.0f, 5.0f),
		RandomFloat(-5.0f, 5.0f),
		RandomFloat(-5.0f, 5.0f)
	);
	pos += randomPos;

	auto& newEnemy = enemyList_.emplace_back(std::make_unique<Enemy>());
	newEnemy->Init();
	newEnemy->GetTransform()->SetTranslaion(pos);
}

#ifdef _DEBUG
void EnemyManager::Debug_Gui() {
	if (ImGui::TreeNode("EnemyManager")) {

		ImGui::Begin("EnmeyManager");
		ImGui::Checkbox("isPop", &isPop_);
		ImGui::End();
		
		ImGui::TreePop();
	}
}
#endif