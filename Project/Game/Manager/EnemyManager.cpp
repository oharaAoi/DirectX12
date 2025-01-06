#include "EnemyManager.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Math/MyRandom.h"

EnemyManager::EnemyManager() {}
EnemyManager::~EnemyManager() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Init() {
	normalEnemyList_.clear();

	Pop(Vector3(0.0f, 0.0f, 10.0f));
	Pop(Vector3(9.0f, 0.0f, 15.0f));
	Pop(Vector3(-9.0f, 0.0f, 15.0f));

	downNum_ = 49;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&EnemyManager::Debug_Gui, this), "EnemyManager");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Update() {
	normalEnemyList_.remove_if([this](const std::shared_ptr<NormalEnemy>& enemy) {
		if (enemy->GetIsDead()) {
			this->CheckNearEnemyList();
			downNum_++;
			return true; // 削除
		}
		return false; // 削除しない
	});

	for (std::list<std::shared_ptr<NormalEnemy>>::iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		(*it)->SetPlayerPos(playerPos_);
		(*it)->Update();
		++it;
	}

	CheckNearEnemyList();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Draw() const {
	for (std::list<std::shared_ptr<NormalEnemy>>::const_iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		(*it)->Draw();
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::Pop(const Vector3& pos) {
	auto& newEnemy = normalEnemyList_.emplace_back(std::make_shared<NormalEnemy>());
	newEnemy->Init();
	newEnemy->GetTransform()->SetTranslaion(pos);
}

void EnemyManager::RandomPop() {
	if (isStop_) {
		return;
	}

	popTime_ += GameTimer::DeltaTime();

	if (popTime_ >= popTimeLimit_) {
		for (int oi = 0; oi < popNum_; ++oi) {
			Pop(Vector3(
				RandomFloat(-20.0f, 20.0f),
				RandomFloat(-20.0f, 20.0f),
				RandomFloat(-20.0f, 20.0f)
			));
		}
		popTime_ = 0.0f;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　一番近い敵を探索する
/////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyManager::CheckNearEnemyList() {
	nearNormalEnemyList_.clear();
	// 一旦すべて格納する
	for (std::list<std::shared_ptr<NormalEnemy>>::iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		nearNormalEnemyList_.emplace_back((*it));
		++it;
	}

	Vector3 pos = playerPos_;
	// ソートする
	std::sort(
		nearNormalEnemyList_.begin(), nearNormalEnemyList_.end(),
		[pos](std::shared_ptr<NormalEnemy> enemy1, std::shared_ptr<NormalEnemy> enemy2) {
			return (enemy1->GetTransform()->GetTranslation() - pos).Length() < (enemy2->GetTransform()->GetTranslation() - pos).Length();
		}
	);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　近い敵のポインタを返す
/////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<NormalEnemy> EnemyManager::GetNearEnemy(uint32_t& index) {
	// 指定された値が配列を超えていないかをチェックする
	if (index < nearNormalEnemyList_.size()) {
		return nearNormalEnemyList_[index];
	} else {
		index = 0;
		return nearNormalEnemyList_[0];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void EnemyManager::Debug_Gui() {
	if (ImGui::CollapsingHeader("popInformation")) {
		ImGui::Checkbox("isStop", &isStop_);
		ImGui::SliderFloat("popTime", &popTime_, 0.0f, popTimeLimit_);
		ImGui::DragFloat("popTimeLimit", &popTimeLimit_, 1.0f);

		ImGui::SliderInt("popNum", &popNum_, 0, 10);
	}

	if (ImGui::Button("pop")) {
		Pop(newEnemyPos_);
	}
	ImGui::SameLine();
	ImGui::DragFloat3("newEnemyPos", &newEnemyPos_.x, 0.1f);

	if (ImGui::CollapsingHeader("enemies")) {
		uint32_t index = 0;
		for (auto& enemy : normalEnemyList_) {
			std::string label = "enemy" + std::to_string(index);
			if (ImGui::TreeNode(label.c_str())) {
				enemy->Debug_Gui();
				ImGui::TreePop();
			}
			index++;
		}
	}
}
void EnemyManager::Debug_Draw() {
	for (std::list<std::shared_ptr<NormalEnemy>>::const_iterator it = normalEnemyList_.begin(); it != normalEnemyList_.end();) {
		(*it)->Debug_Draw();
		++it;
	}
}
#endif