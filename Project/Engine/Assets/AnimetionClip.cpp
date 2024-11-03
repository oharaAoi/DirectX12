#include "AnimetionClip.h"
#include "Engine/Manager/AnimationManager.h"

AnimetionClip::AnimetionClip() {
	manager_ = AnimationManager::GetInstance();

	isAnimationChange_ = false;
	blendFactor_ = 0.0f;
}

AnimetionClip ::~AnimetionClip() {
}

void AnimetionClip::Init() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimetionClip::Update() {
	isAnimationFinish_ = false;
	animationTime_ += GameTimer::DeltaTime();
	// アニメーションが終了したら
	if (animationTime_ >= animation_.duration) {
		isAnimationFinish_ = true;
		animationTime_ = std::fmod(animationTime_, animation_.duration);
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

void AnimetionClip::LoadAnimation(const std::string directoryPath, const std::string& animationFile, const std::string& rootName, bool isSkinning) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + animationFile;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);		// アニメーションがない
	
	Log("Start LoadAnimation[" + animationFile + "]\n");

	std::unordered_map<std::string, Animation> animationMap;

	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex) {
		// sceneからanimationの情報を取得する
		aiAnimation* animationAssimp = scene->mAnimations[animationIndex];

		std::string animationName = animationAssimp->mName.C_Str();										// animationの名前
		Animation animationData;																		// animationのデータ
		animationData.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	// 時間の単位を秒に変換

		//animation_.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	// 時間の単位を秒に変換

		// -------------------------------------------------
		// ↓ アニメーションの解析
		// -------------------------------------------------
		// assimpでは個々のNodeのAnimationをchannelと呼ぶ
		for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
			aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
			NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

			rootName_ = nodeAnimationAssimp->mNodeName.C_Str();

			// -------------------------------------------------
			// ↓ Vector3の読み込み
			// -------------------------------------------------
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
				KeyframeVector3 keyframe;
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.translate.keyframes.push_back(keyframe);
			}

			// -------------------------------------------------
			// ↓ Quaternionの読み込み
			// -------------------------------------------------
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
				aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
				KeyframeQuaternion keyframe;
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
				nodeAnimation.rotate.keyframes.push_back(keyframe);
			}

			// -------------------------------------------------
			// ↓ Scaleの読み込み
			// -------------------------------------------------
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
				KeyframeVector3 keyframe;
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.scale.keyframes.push_back(keyframe);
			}
		}

		animationMap.try_emplace(animationName, animationData);

		/*animations_.push_back(animation);*/
	}

	Log("End LoadAnimation[" + animationFile + "]\n");

	manager_->AddMap(animationMap, animationFile);

	animationFileName_ = animationFile;

	isSkinnig_ = isSkinning;

	// 先頭のアニメーションを追加しておく
	animation_ = manager_->GetAnimation(animationFile, manager_->GetAnimationFirstName(animationFile));

	animationNames_ = manager_->GetModelHaveAnimationNames(animationFileName_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationの取得
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimetionClip::LoadGetAnimation(const std::string& animationFile, bool isSkinning) {
	animation_ = manager_->GetAnimation(animationFile, manager_->GetAnimationFirstName(animationFile));
	isSkinnig_ = isSkinning;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　animationの適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void AnimetionClip::ApplyAnimation(Skeleton* skelton) {
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

void AnimetionClip::LerpApplyAnimation(Skeleton* skelton) {
	blendFactor_ += GameTimer::DeltaTime();
	lerpAnimationTime_[1] += GameTimer::DeltaTime();

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　補完(Vector3)
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 AnimetionClip::CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time) {
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

Quaternion AnimetionClip::CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, const float& time) {
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

void AnimetionClip::SetLerpAnimation(const std::string& preAnimation, const std::string& lerpAnimation) {
	lerpAnimetion_[0] = manager_->GetAnimation(animationFileName_, preAnimation);
	lerpAnimetion_[1] = manager_->GetAnimation(animationFileName_, lerpAnimation);

	lerpAnimationTime_[0] = animationTime_;
	lerpAnimationTime_[1] = 0;

	isAnimationChange_ = true;

	blendFactor_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void AnimetionClip::Debug_Gui() {
	bool isChange = false;
	if (ImGui::TreeNode("animation")) {
		// 現在のanimationを設定する
		if (ImGui::BeginCombo("Select Animation", animationNames_[selectedAnimationIndex].c_str())) {
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

		// 遷移前アニメーション
		if (ImGui::BeginCombo("before Animation", animationNames_[lerpAnimationNamesIndex_[0]].c_str())) {
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
		if (ImGui::BeginCombo("after Animation", animationNames_[lerpAnimationNamesIndex_[1]].c_str())) {
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
		ImGui::TreePop();
	}

	if (ImGui::Button("isChange")) {
		SetLerpAnimation(
			animationNames_[lerpAnimationNamesIndex_[0]],
			animationNames_[lerpAnimationNamesIndex_[1]]
		);
	}

	if (isChange) {
		std::string animationName = animationNames_[selectedAnimationIndex];
		animation_ = manager_->GetAnimation(animationFileName_, animationName);
	}
}
#endif // DEBUG
