#include "EnemyManager.h"
#include "Engine/Editer/Window/EditerWindows.h"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Init() {
	normalEnemyList_.clear();

	Pop(Vector3(0.0f, 0.0f, 10.0f));

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&EnemyManager::Debug_Gui, this), "EnemyManager");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Update() {
	for (std::list<std::unique_ptr<NormalEnemy>>::iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		(*it)->Update();
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Draw() const {
	for (std::list<std::unique_ptr<NormalEnemy>>::const_iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		(*it)->Draw();
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Pop(const Vector3& pos) {
	auto& newEnemy = normalEnemyList_.emplace_back(std::make_unique<NormalEnemy>());
	newEnemy->Init();
	newEnemy->GetTransform()->SetTranslaion(pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void EnemyManager::Debug_Gui() {
	if (ImGui::Button("pop")) {
		Pop(newEnemyPos_);
	}
	ImGui::SameLine();
	ImGui::DragFloat3("newEnemyPos", &newEnemyPos_.x, 0.1f);
}
#endif