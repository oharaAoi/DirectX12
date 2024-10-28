#pragma once
#include <vector>
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/Assets/AnimetionClip.h"

class AnimationManager {
public:

	AnimationManager() = default;
	~AnimationManager();
	AnimationManager(const AnimationManager&) = delete;
	const AnimationManager& operator=(const AnimationManager&) = delete;

	static AnimationManager* GetInstance();

	void Init();

	void AddMap(const AnimetionClip::Animation& animation, const std::string& name);

	AnimetionClip::Animation GetAnimation(const std::string& name);

	bool CheckAnimation(const std::string& name);

private:

	std::map<std::string, AnimetionClip::Animation> animationMap_;



};

