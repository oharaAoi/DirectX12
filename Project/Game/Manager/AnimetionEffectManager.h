#pragma once
#include <list>
#include <vector>
#include "Engine/GameObject/BaseGameObject.h"

class AnimetionEffectManager {
public:

	struct EffectData {
		std::unique_ptr<BaseGameObject> effect;
		const Vector3* pTranslation;
		const Quaternion* pQuaternion;
	};


public:

	AnimetionEffectManager() = default;
	~AnimetionEffectManager();
	AnimetionEffectManager(const AnimetionEffectManager&) = delete;
	const AnimetionEffectManager& operator=(const AnimetionEffectManager&) = delete;

	static AnimetionEffectManager* GetInstance();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// Effectを作成してリストに追加
	/// </summary>
	/// <param name="directoryPath">: ディレクトリパス</param>
	/// <param name="filePath">: ファイルパス</param>
	/// <param name="parentWorldTransform">: 親がいるならworldTransformのポインタを渡す</param>
	/// <param name="isLighting">: ライティングをするか</param>
	/// <param name="scale">: scale量</param>
	/// <param name="rotate">: 回転</param>
	/// <param name="translation">: 平行移動</param>
	static void AddListEffect(const std::string& directoryPath, const std::string& filePath,
							  const WorldTransform* parentWorldTransform,bool isLighting,
							  const Vector3& scale, const Quaternion& rotate, const Vector3& translation);

	/// <summary>
	///  Effectを作成してリストに追加
	/// </summary>
	/// <param name="directoryPath">: ディレクトリパス</param>
	/// <param name="filePath">: ファイルパス</param>
	/// <param name="parentWorldTransform">: 親がいるならworldTransformのポインタを渡す</param>
	/// <param name="isParent">: 親子関係を結ぶか</param>
	/// <param name="isLighting">: ライティングをするか</param>
	/// <param name="scale">: scaleを大きくするか(行わないなら引数なし)</param>
	/// <param name="rotate">: 回転をかける量(行わないなら引数なし)</param>
	/// <param name="translation">: 移動させる量(行わないなら引数なし)</param>
	/// 
private:

	std::list<EffectData> effectList_;

};

