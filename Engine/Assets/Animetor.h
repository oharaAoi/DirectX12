#pragma once
#include <memory>
#include "Engine.h"

class AnimetionClip;
class Skeleton;
class Skinning;

/// <summary>
/// SkeletonやSkinningの情報を持っているクラス
/// </summary>
class Animetor {
public:

	Animetor();
	~Animetor();

	void Update();

	void LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model);

public:

	const Skinning* GetSkinning() { return skinning_.get(); }
	
private:

	std::unique_ptr<AnimetionClip> animetionClip_;
	std::unique_ptr<Skeleton> skeleton_ = nullptr;
	std::unique_ptr<Skinning> skinning_ = nullptr;
};

