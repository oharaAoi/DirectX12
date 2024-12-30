#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Assets/Rigging/Skeleton.h"
#include "Engine/Assets/Animation/AnimationClip.h"

class AnimationManager {
public:

	AnimationManager() = default;
	~AnimationManager();
	AnimationManager(const AnimationManager&) = delete;
	const AnimationManager& operator=(const AnimationManager&) = delete;

	static AnimationManager* GetInstance();

	void Init();

	/// <summary>
	/// Animationの追加
	/// </summary>
	/// <param name="animation"></param>
	/// <param name="name"></param>
	void AddMap(const std::unordered_map<std::string, AnimationClip::Animation>& animation, const std::string& name);

	/// <summary>
	/// 指定されたAnimationを取得する
	/// </summary>
	/// <param name="animationFile">: animationが格納されているファイル名</param>
	/// <param name="animationName">: 行いたいanimation名</param>
	/// <returns></returns>
	AnimationClip::Animation GetAnimation(const std::string& animationFile, const std::string& animationName);

	std::string GetAnimationFirstName(const std::string& animationFileName);

	std::vector<std::string>& GetModelHaveAnimationNames(const std::string& animationFileName) { return modelHaveAnimationNames_[animationFileName]; }

	bool CheckAnimation(const std::string& name);

private:

	std::unordered_map<std::string, std::unordered_map<std::string, AnimationClip::Animation>> animationMap_;

	std::unordered_map<std::string, std::vector<std::string>> modelHaveAnimationNames_;

};

