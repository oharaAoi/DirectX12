#include "PlayerAnimator.h"
#include "Game/GameObject/Player/Player.h"

PlayerAnimator::~PlayerAnimator() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAnimator::Init() {
	Load();

	animtionTime_ = 0.0f;
	if (pPlayer_->GetAnimetor() != nullptr) {
		pPlayerAnimator_ = pPlayer_->GetAnimetor();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAnimator::Update() {
	if (pPlayerAnimator_ != nullptr) {
		//animtionTime_ += GameTimer::DeltaTime();
		animtionTime_ = fmod(animtionTime_, pPlayerAnimator_->GetAnimationDuration());
		pPlayerAnimator_->UpdateScript(animtionTime_);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　遷移
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAnimator::Transition(const std::string& before, const std::string after) {
	pPlayer_->GetAnimetor()->SetTransitionAnimation(stateAnimationMap_[before], stateAnimationMap_[after]);
	nowState_ = after;
}

void PlayerAnimator::NowToAfterTransition(const std::string afterState) {
	// 遷移先が同じなら何もしない
	if (nowState_ == afterState) {
		return;
	}

	pPlayer_->GetAnimetor()->SetTransitionAnimation(stateAnimationMap_[nowState_], stateAnimationMap_[afterState]);
	nowState_ = afterState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAnimator::Load() {
	std::string filePath = kDirectoryPath + "playerStateAnimation" + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist playerStateAnimation.json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// すべてのキーと値を出力
	for (auto& [topKey, nestedData] : root.items()) {
		for (auto& [key, value] : nestedData.items()) {
			stateAnimationMap_.emplace(key, value);
		}
	}
}

#ifdef _DEBUG
void PlayerAnimator::Debug_Gui() {
	if (ImGui::TreeNode("playerAnimator")) {
		if (ImGui::Button("create State")) {
			popCreateWindow_ = true;
		}
		// 新しく生成するwindowを出す
		if (popCreateWindow_) {
			Debug_CreateState();
		}

		// 生成した状態のmapを表示
		for (auto it = debug_stateAnimationMap_.begin(); it != debug_stateAnimationMap_.end();) {
			if (ImGui::TreeNode((*it).first.c_str())) {
				std::string animation = "animation : " + (*it).second;
				ImGui::Text(animation.c_str());

				if (ImGui::Button("Delete")) {
					it = debug_stateAnimationMap_.erase(it);
					ImGui::TreePop();
					continue;
				}
				ImGui::TreePop();
			}
			++it;
		}

		if (ImGui::Button("Save File")) {
			Save();
		}

		ImGui::TreePop();
	}
}


void PlayerAnimator::Debug_CreateState() {
	ImGui::Begin("create Animation State");
	ImGui::InputText("stateName", &stateName_[0], sizeof(char) * 64);
	pPlayer_->GetAnimetor()->GetAnimationClip()->Debug_Select();
	selectName_ = pPlayer_->GetAnimetor()->GetDebugAnimationName();

	if (ImGui::Button("Add")) {
		if (debug_stateAnimationMap_.find(stateName_) == debug_stateAnimationMap_.end()) {
			// キーが存在しない場合のみ追加
			debug_stateAnimationMap_.emplace(stateName_.c_str(), selectName_);
		} else {
			// デバッグ用：キーが既に存在する場合のログ
			std::cout << "Key already exists: " << stateName_ << std::endl;
		}
		popCreateWindow_ = false;
	}
	ImGui::End();
}


void PlayerAnimator::Save() {
	json root;
	for (auto it = debug_stateAnimationMap_.begin(); it != debug_stateAnimationMap_.end(); ++it) {
		root["player"][(*it).first.c_str()] = (*it).second;
	}

	// 実際にファイルに保存する
	std::string filePath = kDirectoryPath + "playerStateAnimation" + ".json";

	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (!ofs.fail()) {
		assert("not open File");
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}
#endif