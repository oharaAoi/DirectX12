#pragma once
#include <map>
#include <string>
#include <assert.h>
#include "Engine/Audio/Audio.h"
#include "Engine/Engine.h"

class AudioManager {
public:

	AudioManager() = default;
	~AudioManager();
	AudioManager(const AudioManager&) = delete;
	const AudioManager& operator=(const AudioManager&) = delete;

	static AudioManager* GetInstance();

	void Init();

	/// <summary>
	/// Audioをmapに追加する
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	void AddMap(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// Audioを構成するデータを取得
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	SoundData GetAudioData(const std::string& fileName);

	/// <summary>
	/// Audioを読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	static void LoadAudio(const std::string& directoryPath, const std::string& fileName);

private:

	std::map<std::string, const SoundData> audioLoadData_;

};

