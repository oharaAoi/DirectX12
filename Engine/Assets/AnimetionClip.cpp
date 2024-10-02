#include "AnimetionClip.h"

AnimetionClip::AnimetionClip() {
}

AnimetionClip ::~AnimetionClip() {
}

void AnimetionClip::Init() {
}

void AnimetionClip::Update() {
	animationTime_ += 1.0f / 60.0f;
	animationTime_ = std::fmod(animationTime_, animation_.duration);

	////// =======================================================================================
	//NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[rootName];
	////// =======================================================================================

	//Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
	//Quaternion rotate = CalculateQuaternion(rootNodeAnimation.rotate.keyframes, animationTime_);
	//Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
	//animationMat_ = MakeAffineMatrix(scale, rotate, translate);
}

void AnimetionClip::LoadAnimation(const std::string directoryPath, const std::string& animationFile) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + animationFile;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);		// アニメーションがない
	// =======================================================================================
	aiAnimation* animationAssimp = scene->mAnimations[0];	// 最初のアニメーションだけ	
	// =======================================================================================
	animation_.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	// 時間の単位を秒に変換

	// -------------------------------------------------
	// ↓ アニメーションの解析
	// -------------------------------------------------
	// assimpでは個々のNodeのAnimationをchannelと呼ぶ
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation_.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

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
}

void AnimetionClip::ApplyAnimation(Skeleton& skelton) {
	for (Skeleton::Joint& joint : skelton.GetJoints()) {
		if (auto it = animation_.nodeAnimations.find(joint.name); it != animation_.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform.rotate = CalculateQuaternion(rootNodeAnimation.rotate.keyframes, animationTime_);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		}
	}
}

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