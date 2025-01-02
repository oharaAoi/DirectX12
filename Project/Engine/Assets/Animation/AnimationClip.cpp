#include "AnimationClip.h"
#include "Engine/System/Manager/AnimationManager.h"

AnimationClip::AnimationClip() {
	manager_ = AnimationManager::GetInstance();

	isAnimationChange_ = false;
	blendFactor_ = 0.0f;
}

AnimationClip ::~AnimationClip() {
}

void AnimationClip::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::Update() {
	nowAnimationName_ = animation_.animationName;
	if (isStop_) {
		return;
	}

	// アニメーションが終了したら
	if (isAnimationFinish_) {
		if (isLoop_) {
			isAnimationFinish_ = false;
			animationTime_ = std::fmod(animationTime_, animation_.duration);
		}
	}

	// animationを進める
	animationTime_ += GameTimer::DeltaTime() * animationSpeed_;
	if (animationTime_ >= animation_.duration) {
		isAnimationFinish_ = true;
	}

	// 遷移の予約が入っていたら
	if (isReservation_) {
		if ((animationTime_ / animation_.duration) >= startTransitionRaito_) {
			isAnimationChange_ = true;
			isReservation_ = false;
			return;
		}
	}
	
	// skinningを行わない場合アニメーションの行列を更新する
	if (!isSkinnig_) {
		// =======================================================================================
		NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[rootName_];
		// =======================================================================================

		Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
		Quaternion rotate = CalculateQuaternion(rootNodeAnimation.rotate.keyframes, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		animationMat_ = Matrix4x4::MakeAffine(scale, rotate, translate);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::LoadAnimation(const std::string directoryPath, const std::string& animationFile, const std::string& rootName, bool isSkinning) {
	animationFileName_ = animationFile;
	isSkinnig_ = isSkinning;
	rootName_ = rootName;
	
	Assimp::Importer importer;
	std::string filePath = directoryPath + animationFile;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	if (!scene || scene->mNumAnimations == 0) {
		throw std::runtime_error("Failed to load animations or no animations present");
	}
	
	Log("Start LoadAnimationFile [" + animationFile + "]\n");

	std::unordered_map<std::string, Animation> animationMap{};

	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex) {
		// sceneからanimationの情報を取得する
		aiAnimation* animationAssimp = scene->mAnimations[animationIndex];

		std::string animationName = animationAssimp->mName.C_Str();										// animationの名前
		Animation animationData{};																		// animationのデータ
		animationData.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	// 時間の単位を秒に変換
		animationData.animationName = animationName;													// animatonの名前を取得

		Log("LoadAnimation[" + animationName + "]\n");

		// -------------------------------------------------
		// ↓ アニメーションの解析
		// -------------------------------------------------
		// assimpでは個々のNodeのAnimationをchannelと呼ぶ
		for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
			aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
			NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

			// -------------------------------------------------
			// ↓ Vector3の読み込み
			// -------------------------------------------------
			Log("\n");
			Log("[animation Translate]\n");
			Log("\n");
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
				KeyframeVector3 keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.translate.keyframes.push_back(keyframe);
				/*Log("---------------------------------\n");
				std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);

				std::string valueXLog = "keyFrame.value X : " + std::to_string(keyframe.value.x) + "\n";
				std::string valueYLog = "keyFrame.value Y : " + std::to_string(keyframe.value.y) + "\n";
				std::string valueZLog = "keyFrame.value Z : " + std::to_string(keyframe.value.z) + "\n";
				
				Log(valueXLog);
				Log(valueYLog);
				Log(valueZLog);*/
			}

			// -------------------------------------------------
			// ↓ Quaternionの読み込み
			// -------------------------------------------------
			Log("\n");
			Log("[animation Rotate]\n");
			Log("\n");
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {

				aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
				KeyframeQuaternion keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
				nodeAnimation.rotate.keyframes.push_back(keyframe);
				/*std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);*/
				/*Log("--------------------------------------------------\n");
				std::string timeLog = "TicksPerSecond : " + std::to_string(animationAssimp->mTicksPerSecond) + "\n";
				Log(timeLog);
				
				Log("--------------------------------------------------\n");*/
	
			}

			// -------------------------------------------------
			// ↓ Scaleの読み込み
			// -------------------------------------------------
			Log("\n");
			Log("[animation Scale]\n");
			Log("\n");
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
				KeyframeVector3 keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.scale.keyframes.push_back(keyframe);

				Log("---------------------------------\n");
				std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);

				std::string valueXLog = "keyFrame.value X : " + std::to_string(keyframe.value.x) + "\n";
				std::string valueYLog = "keyFrame.value Y : " + std::to_string(keyframe.value.y) + "\n";
				std::string valueZLog = "keyFrame.value Z : " + std::to_string(keyframe.value.z) + "\n";

				Log(valueXLog);
				Log(valueYLog);
				Log(valueZLog);
			}
		}

		animationMap.try_emplace(animationName, animationData);

		/*animations_.push_back(animation);*/
	}

	Log("End LoadAnimation[" + animationFile + "]\n");

	// managerにanimationデータを追加
	manager_->AddMap(animationMap, animationFile);
	// 先頭のアニメーションを追加しておく
	nowAnimationName_ = manager_->GetAnimationFirstName(animationFile);
	animation_ = manager_->GetAnimation(animationFile, nowAnimationName_);
	// すべてのanimationの名前を取得
	animationNames_ = manager_->GetModelHaveAnimationNames(animationFileName_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationの取得
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::LoadGetAnimation(const std::string& animationFile, bool isSkinning) {
	animationFileName_ = animationFile;
	nowAnimationName_ = animationFile;
	isSkinnig_ = isSkinning;

	nowAnimationName_ = manager_->GetAnimationFirstName(animationFile);
	animation_ = manager_->GetAnimation(animationFile, nowAnimationName_);
	// すべてのanimationの名前を取得
	animationNames_ = manager_->GetModelHaveAnimationNames(animationFileName_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationの適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::ApplyAnimation(Skeleton* skelton) {
	for (Skeleton::Joint& joint : skelton->GetJoints()) {
		if (auto it = animation_.nodeAnimations.find(joint.name); it != animation_.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform.rotate = CalculateQuaternion(rootNodeAnimation.rotate.keyframes, animationTime_);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationを遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::LerpApplyAnimation(Skeleton* skelton) {
	if (isReservation_) {
		animationTime_ += GameTimer::DeltaTime() * animationSpeed_;
	}

	blendFactor_ += GameTimer::DeltaTime() * animationSpeed_;
	lerpAnimationTime_[1] += GameTimer::DeltaTime() * animationSpeed_;

	for (Skeleton::Joint& joint : skelton->GetJoints()) {

		auto itA = lerpAnimetion_[0].nodeAnimations.find(joint.name);
		// アニメーションBのノードアニメーションを取得
		auto itB = lerpAnimetion_[1].nodeAnimations.find(joint.name);

		if (itA != lerpAnimetion_[0].nodeAnimations.end() && itB != lerpAnimetion_[1].nodeAnimations.end()) {
			const NodeAnimation& nodeAnimationA = itA->second;
			const NodeAnimation& nodeAnimationB = itB->second;

			// アニメーションAとBのそれぞれの変換を取得
			Vector3 translateA = CalculateValue(nodeAnimationA.translate.keyframes, animationTime_);
			Quaternion rotateA = CalculateQuaternion(nodeAnimationA.rotate.keyframes, animationTime_);
			Vector3 scaleA = CalculateValue(nodeAnimationA.scale.keyframes, animationTime_);

			Vector3 translateB = CalculateValue(nodeAnimationB.translate.keyframes, lerpAnimationTime_[1]);
			Quaternion rotateB = CalculateQuaternion(nodeAnimationB.rotate.keyframes, lerpAnimationTime_[1]);
			Vector3 scaleB = CalculateValue(nodeAnimationB.scale.keyframes, lerpAnimationTime_[1]);

			// blendFactor（補間率）に基づいて補間
			float t = blendFactor_ / 1.0f;
			joint.transform.translate = Lerp(translateA, translateB, t);
			joint.transform.rotate = Quaternion::Slerp(rotateA, rotateB, t);
			joint.transform.scale = Lerp(scaleA, scaleB, t);
		}
	}

	// 1以上になったら完全に遷移させる
	if (blendFactor_ >= 1.0f) {
		isAnimationChange_ = false;
		animationTime_ = lerpAnimationTime_[1];
		animation_ = lerpAnimetion_[1];
	}
}


void AnimationClip::AnimationTransition(Skeleton* skelton, float time) {
	blendFactor_ += GameTimer::DeltaTime() / time;
	lerpAnimationTime_[1] += GameTimer::DeltaTime();

	for (Skeleton::Joint& joint : skelton->GetJoints()) {
		auto itA = lerpAnimetion_[0].nodeAnimations.find(joint.name);
		auto itB = lerpAnimetion_[1].nodeAnimations.find(joint.name);

		if (itA != lerpAnimetion_[0].nodeAnimations.end() && itB != lerpAnimetion_[1].nodeAnimations.end()) {
			const NodeAnimation& nodeAnimationA = itA->second;
			const NodeAnimation& nodeAnimationB = itB->second;

			// アニメーションAとBのそれぞれの変換を取得
			Vector3 translateA = CalculateValue(nodeAnimationA.translate.keyframes, animationTime_);
			Quaternion rotateA = CalculateQuaternion(nodeAnimationA.rotate.keyframes, animationTime_);
			Vector3 scaleA = CalculateValue(nodeAnimationA.scale.keyframes, animationTime_);

			Vector3 translateB = CalculateValue(nodeAnimationB.translate.keyframes, lerpAnimationTime_[1]);
			Quaternion rotateB = CalculateQuaternion(nodeAnimationB.rotate.keyframes, lerpAnimationTime_[1]);
			Vector3 scaleB = CalculateValue(nodeAnimationB.scale.keyframes, lerpAnimationTime_[1]);

			// blendFactor（補間率）に基づいて補間
			float t = blendFactor_ / 1.0f;
			joint.transform.translate = Lerp(translateA, translateB, t);
			joint.transform.rotate = Quaternion::Slerp(rotateA, rotateB, t);
			joint.transform.scale = Lerp(scaleA, scaleB, t);
		}
	}

	// 遷移が完了した場合の処理
	if (blendFactor_ >= 1.0f) {
		isAnimationChange_ = false;
		animationTime_ = lerpAnimationTime_[1];
		animation_ = lerpAnimetion_[1];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　補完(Vector3)
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 AnimationClip::CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time) {
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Vector3::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	//　ここまで来たら一番後の時刻よりも後ろなので最後の値を返す
	return (*keyframes.rbegin()).value;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　補完(Quaternion)
//////////////////////////////////////////////////////////////////////////////////////////////////

Quaternion AnimationClip::CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, const float& time) {
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Quaternion::Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	//　ここまで来たら一番後の時刻よりも後ろなので最後の値を返す
	return (*keyframes.rbegin()).value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animation同士の補完を設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationClip::SetLerpAnimation(const std::string& preAnimation, const std::string& lerpAnimation) {
	lerpAnimetion_[0] = manager_->GetAnimation(animationFileName_, preAnimation);
	lerpAnimetion_[1] = manager_->GetAnimation(animationFileName_, lerpAnimation);

	lerpAnimationTime_[0] = lerpAnimetion_[0].duration;
	lerpAnimationTime_[1] = 0;

	isAnimationChange_ = true;

	blendFactor_ = 0.0f;

	nowAnimationName_ = lerpAnimation;
}


void AnimationClip::SetLerpAnimation(const std::string& lerpAnimation) {
	// 今と遷移後がAnimationが同じだったら何もしない
	if (animation_.animationName == lerpAnimation) {
		return;
	}

	lerpAnimetion_[0] = animation_;
	lerpAnimetion_[1] = manager_->GetAnimation(animationFileName_, lerpAnimation);

	lerpAnimationTime_[0] = animationTime_;
	lerpAnimationTime_[1] = 0;

	isAnimationChange_ = true;

	blendFactor_ = 0.0f;

	nowAnimationName_ = lerpAnimation;
}

void AnimationClip::SetAnimation(const std::string& animationName) {
	animation_ = manager_->GetAnimation(animationFileName_, animationName);
	animationTime_ = 0.0f;
}

void AnimationClip::SetAnimationReservation(const std::string& preAnimation, const std::string& lerpAnimation, float startTransitionRaito) {
	lerpAnimetion_[0] = manager_->GetAnimation(animationFileName_, preAnimation);
	lerpAnimetion_[1] = manager_->GetAnimation(animationFileName_, lerpAnimation);

	lerpAnimationTime_[0] = animationTime_;
	lerpAnimationTime_[1] = 0;

	isReservation_ = true;
	startTransitionRaito_ = startTransitionRaito;
	blendFactor_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void AnimationClip::Debug_Gui() {
	bool isChange = false;
	int isAnimationFinish = (int)isAnimationFinish_;
	if (ImGui::TreeNode("animation")) {

		if (ImGui::Button("play")) {
			isStop_ = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("stop")) {
			isStop_ = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("reset")) {
			animationTime_ = 0.0f;
		}

		ImGui::SliderFloat("time", &animationTime_, 0.0f, animation_.duration);
		ImGui::DragFloat("speed", &animationSpeed_, 0.1f);
		ImGui::SliderInt("isFinish", &isAnimationFinish, 0, 1);
		ImGui::Text(nowAnimationName_.c_str());

		{
			ImGui::BulletText("SelectAnimation");
			// 現在のanimationを設定する
			if (ImGui::BeginCombo("## select", animationNames_[selectedAnimationIndex].c_str())) {
				for (int i = 0; i < animationNames_.size(); ++i) {
					bool isSelected = (i == selectedAnimationIndex);
					if (ImGui::Selectable(animationNames_[i].c_str(), isSelected)) {
						selectedAnimationIndex = i; // インデックスを更新
						isChange = true;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus(); // 初期選択のフォーカス
					}
				}
				ImGui::EndCombo();
			}

			if (isChange) {
				std::string animationName = animationNames_[selectedAnimationIndex];
				animation_ = manager_->GetAnimation(animationFileName_, animationName);
			}
		}

		{
			ImGui::BulletText("Transition Animation");
			// 遷移前アニメーション
			if (ImGui::BeginCombo("before", animationNames_[lerpAnimationNamesIndex_[0]].c_str())) {
				for (int i = 0; i < animationNames_.size(); ++i) {
					bool isSelected = (i == lerpAnimationNamesIndex_[0]);
					if (ImGui::Selectable(animationNames_[i].c_str(), isSelected)) {
						lerpAnimationNamesIndex_[0] = i; // インデックスを更新
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus(); // 初期選択のフォーカス
					}
				}
				ImGui::EndCombo();
			}

			// 遷移後アニメーション
			if (ImGui::BeginCombo("after", animationNames_[lerpAnimationNamesIndex_[1]].c_str())) {
				for (int i = 0; i < animationNames_.size(); ++i) {
					bool isSelected = (i == lerpAnimationNamesIndex_[1]);
					if (ImGui::Selectable(animationNames_[i].c_str(), isSelected)) {
						lerpAnimationNamesIndex_[1] = i; // インデックスを更新
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus(); // 初期選択のフォーカス
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("isChange")) {
				SetLerpAnimation(
					animationNames_[lerpAnimationNamesIndex_[0]],
					animationNames_[lerpAnimationNamesIndex_[1]]
				);
			}
		}

		ImGui::TreePop();
	}

	if (isAnimationChange_) {
		ImGui::Text("true Change");
	} else {
		ImGui::Text("not Change");
	}

	ImGui::Text(nowAnimationName_.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ImGuiで選択されているAnimatinoの名前を返す
//////////////////////////////////////////////////////////////////////////////////////////////////

std::string AnimationClip::SelectAnimationName() {
	if (ImGui::BeginCombo("Select Animation", animationNames_[selectedAnimationIndex].c_str())) {
		for (int i = 0; i < animationNames_.size(); ++i) {
			bool isSelected = (i == selectedAnimationIndex);
			if (ImGui::Selectable(animationNames_[i].c_str(), isSelected)) {
				selectedAnimationIndex = i; // インデックスを更新
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus(); // 初期選択のフォーカス
			}
		}
		ImGui::EndCombo();
	}
	
	return animationNames_[selectedAnimationIndex];
}
#endif // DEBUG
