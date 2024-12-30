#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {
}

AudioPlayer::~AudioPlayer() {
	Finalize();
}

void AudioPlayer::Finalize() { 
	if (audio_.pSourceVoice != nullptr) {
		Engine::Stop(audio_);
		audio_.pSourceVoice->DestroyVoice();
		audio_.pSourceVoice = nullptr;
	}
}

void AudioPlayer::Init(const std::string& filePath) {
	audio_ = Engine::LoadAudio(AudioManager::GetInstance()->GetAudioData(filePath));
}

void AudioPlayer::Play(bool isLoop, float volume, bool checkPlaying) {
	Engine::PlayAudio(audio_, isLoop, volume, checkPlaying);
}

void AudioPlayer::Pause() {
	Engine::Pause(audio_);
}

void AudioPlayer::ReStart() {
	Engine::ReStart(audio_);
}

void AudioPlayer::Stop() {
	Engine::Stop(audio_);
}

void AudioPlayer::SetVolume(float volume) {
	Engine::SetVolume(audio_, volume);
}

bool AudioPlayer::GetIsPlaying() {
	return Engine::GetIsPlaying(audio_);
}

void AudioPlayer::SinglShotPlay(const std::string& filePath, float volume) {
	Engine::SingleShotPlay(AudioManager::GetInstance()->GetAudioData(filePath), volume);
}
