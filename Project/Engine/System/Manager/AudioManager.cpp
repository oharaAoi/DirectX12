#include "AudioManager.h"


AudioManager::~AudioManager() {
	for (auto& pair : audioLoadData_) {
		delete[] pair.second.pBuffer; 
	}
	audioLoadData_.clear();
}

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Init() {
	audioLoadData_.clear();
}

void AudioManager::AddMap(const std::string& directoryPath, const std::string& fileName) {
	std::string name = directoryPath + fileName;
	if (auto it = audioLoadData_.find(fileName); it != audioLoadData_.end()) {
		return;
	}
	audioLoadData_.try_emplace(fileName, Engine::LoadAudio(name));
}

SoundData AudioManager::GetAudioData(const std::string& fileName) {
	if (auto it = audioLoadData_.find(fileName); it == audioLoadData_.end()) {
		assert(false && "Audio not found!");
	}

	return audioLoadData_[fileName];
}

void AudioManager::LoadAudio(const std::string& directoryPath, const std::string& fileName) {
	GetInstance()->AddMap(directoryPath, fileName);
}
