#pragma once
#include "Engine/Engine.h"
#include "Engine/Audio/Audio.h"
#include "Engine/Manager/AudioManager.h"

class AudioPlayer {
public:

	AudioPlayer();
	~AudioPlayer();

	void Finalize();
	void Init(const std::string& filePath);
	/// <summary>
	/// 音をプレイする
	/// </summary>
	/// <param name="isLoop">: ループするか</param>
	/// <param name="volume">: 音量</param>
	/// <param name="checkPlaying">: プレイ中かの判定を行うかどうか(デフォルトではfalse)</param>
	void Play(bool isLoop, float volume, bool checkPlaying = false);

	/// <summary>
	/// 一時停止
	/// </summary>
	void Pause();

	/// <summary>
	/// Restart
	/// </summary>
	void ReStart();

	/// <summary>
	/// 停止
	/// </summary>
	void Stop();

	/// <summary>
	/// 音量を変える
	/// </summary>
	/// <param name="volume"></param>
	void SetVolume(float volume);

	/// <summary>
	/// 音声がなっているか
	/// </summary>
	/// <returns></returns>
	bool GetIsPlaying();

	static void SinglShotPlay(const std::string& filePath, float volume);

private:

	AudioData audio_;

};

