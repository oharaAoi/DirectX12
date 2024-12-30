#include "Audio.h"

float Audio::masterVolume_ = 0.5f;

Audio::~Audio() {
	xAudio2_.Reset();
}

void Audio::Finalize() {
	for (IXAudio2SourceVoice* sourceVoice : playingSourceList_) {
		if (sourceVoice) {
			sourceVoice->DestroyVoice();  // ボイスを解放
		}
	}
	playingSourceList_.clear();
}

/// <summary>
/// 初期化
/// </summary>
void Audio::Init() {
	HRESULT result = S_FALSE;

	// XAudioエンジンのインスタンス
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void Audio::Update() {
	for (auto it = playingSourceList_.begin(); it != playingSourceList_.end(); ) {
		XAUDIO2_VOICE_STATE state;
		(*it)->GetState(&state);

		if (state.BuffersQueued == 0) {
			// ボイスが再生終了した場合
			(*it)->DestroyVoice();  // リソースの解放
			it = playingSourceList_.erase(it); // リストから削除
		} else {
			++it; // 次のボイスへ
		}
	}
}

/// <summary>
/// 音源のデータを読む
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
SoundData Audio::LoadWave(const char* filename) {
	// --------------------------------------------------
	// ファイルオープン
	std::ifstream file;
	// wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	// --------------------------------------------------
	// wavデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.Chunk.id, "RIFF", 4) != 0) {	//サイズが4バイト
		assert(0);
	}

	// タイプがwaveかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// -------------------------------------------------
	// ↓ Formatチャンクの読み込み
	// -------------------------------------------------
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	while (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		// チャンクサイズ分だけ読み飛ばす
		file.seekg(format.chunk.size, std::ios_base::cur);

		// 次のチャンクヘッダーを読み込む
		file.read((char*)&format.chunk, sizeof(ChunkHeader));

		// ファイルの終端に到達したらエラー
		if (file.eof()) {
			assert(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// -------------------------------------------------
	// ↓ Dataチャンクの読み込み
	// -------------------------------------------------
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	while (strncmp(data.id, "data", 4) != 0) {
		// チャンクサイズ分だけ読み飛ばす
		file.seekg(data.size, std::ios_base::cur);

		// 次のチャンクヘッダーを読み込む
		file.read((char*)&data, sizeof(data));

		// ファイルの終端に到達したらエラー
		if (file.eof()) {
			assert(0);
		}
	}
	
	// 本物のデータチャンク
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// waveファイルを閉じる
	file.close();

	SoundData loadData;
	loadData.wfex = format.fmt;
	loadData.pBuffer = (BYTE*)pBuffer;
	loadData.bufferSize = data.size;

	return loadData;
}

SoundData Audio::LoadMP3(const wchar_t* filename) {

	HRESULT hr = MFStartup(MF_VERSION);
	if(FAILED(hr)) {
		throw std::runtime_error("Media Foundation initialization failed.");
	}

	IMFSourceReader* pReader = nullptr;
	IMFMediaType* pOutputType = nullptr;

	// MP3ファイルのSource Readerを作成
	hr = MFCreateSourceReaderFromURL(filename, nullptr, &pReader);
	if(FAILED(hr)) {
		MFShutdown();
		throw std::runtime_error("Failed to create Source Reader.");
	}

	// 出力タイプをPCM (WAV) に設定
	hr = MFCreateMediaType(&pOutputType);
	if (FAILED(hr)) {
		pReader->Release();
		MFShutdown();
		throw std::runtime_error("Failed to create output media type.");
	}

	hr = pOutputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	hr = pOutputType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	hr = pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pOutputType);

	if (FAILED(hr)) {
		pOutputType->Release();
		pReader->Release();
		MFShutdown();
		throw std::runtime_error("Failed to set media type.");
	}

	pOutputType->Release();
	pOutputType = nullptr;
	pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pOutputType);
	// サンプルを読み取ってデータを抽出
	std::vector<BYTE> audioData;

	while (true) {
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		audioData.resize(audioData.size() + cbCurrentLength);
		memcpy(audioData.data() + audioData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	// SoundDataにデータを格納
	SoundData soundData;
	soundData.pBuffer = new BYTE[audioData.size()];
	std::copy(audioData.begin(), audioData.end(), soundData.pBuffer);
	soundData.bufferSize = static_cast<uint32_t>(audioData.size());

	// フォーマット情報を取得
	hr = pOutputType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, reinterpret_cast<UINT32*>(&soundData.wfex.nChannels));
	hr = pOutputType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, reinterpret_cast<UINT32*>(&soundData.wfex.nSamplesPerSec));
	hr = pOutputType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, reinterpret_cast<UINT32*>(&soundData.wfex.wBitsPerSample));

	soundData.wfex.wFormatTag = WAVE_FORMAT_PCM;
	soundData.wfex.nBlockAlign = soundData.wfex.nChannels * soundData.wfex.wBitsPerSample / 8;
	soundData.wfex.nAvgBytesPerSec = soundData.wfex.nSamplesPerSec * soundData.wfex.nBlockAlign;

	// 後始末
	pOutputType->Release();
	pReader->Release();
	MFShutdown();

	return soundData;
}

AudioData Audio::LoadAudio(const SoundData& loadAudioData) {
	// 読み込んだ音声データをreturn
	AudioData result = {};
	result.data.wfex = loadAudioData.wfex;
	result.data.pBuffer = reinterpret_cast<BYTE*>(loadAudioData.pBuffer);
	result.data.bufferSize = loadAudioData.bufferSize;
	HRESULT hr = xAudio2_->CreateSourceVoice(&result.pSourceVoice, &result.data.wfex);
	assert(SUCCEEDED(hr));

	return result;
}


/// <summary>
/// 音声データの読み込み
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
SoundData Audio::SoundLoad(const char* filename) {
	SoundData loadData{};
	if (std::strcmp(GetFileExtension(filename), "wav") == 0) {
		loadData = LoadWave(filename);
	} else {
		loadData = LoadMP3(ConvertToWideString(filename).c_str());
	}
	// 読み込んだ音声データをreturn
	SoundData soundData = {};
	soundData.wfex = loadData.wfex;
	soundData.pBuffer = reinterpret_cast<BYTE*>(loadData.pBuffer);
	soundData.bufferSize = loadData.bufferSize;
	
	return soundData;
}

/// <summary>
/// 音声データの解放
/// </summary>
/// <param name="soundData"></param>
void Audio::SoundUnload(SoundData* soundData) {
	delete[] soundData->pBuffer;
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

/// <summary>
/// サウンドの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::SoundPlayWave(const SoundData& soundData) {
	HRESULT result = S_FALSE;

	// 波形フォーマットを元にsourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	if (IsPlaying(pSourceVoice)) {
		return;
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	// ループするかしないか
	buf.LoopCount = true;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->SetVolume( masterVolume_);
	result = pSourceVoice->Start();
}

void Audio::PlayAudio(const AudioData& audioData, bool isLoop, float volume, bool checkPlaying) {
	HRESULT result = S_FALSE;

	if (checkPlaying) {
		if (IsPlaying(audioData.pSourceVoice)) {
			return;
		}
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = audioData.data.pBuffer;
	buf.AudioBytes = audioData.data.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	// ループするかしないか
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループ
	} else {
		buf.LoopCount = 0; // ループしない
	}

	// 波形データの再生
	result = audioData.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = audioData.pSourceVoice->SetVolume(volume * masterVolume_);
	assert(SUCCEEDED(result));
	result = audioData.pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

void Audio::SinglShotPlay(const SoundData& loadAudioData, float volume) {
	// 読み込んだ音声データをreturn
	AudioData audio = {};
	audio.data.wfex = loadAudioData.wfex;
	audio.data.pBuffer = reinterpret_cast<BYTE*>(loadAudioData.pBuffer);
	audio.data.bufferSize = loadAudioData.bufferSize;
	HRESULT hr = xAudio2_->CreateSourceVoice(&audio.pSourceVoice, &audio.data.wfex);
	assert(SUCCEEDED(hr));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = audio.data.pBuffer;
	buf.AudioBytes = audio.data.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = 0; // ループしない

	// 波形データの再生
	hr = audio.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));
	hr = audio.pSourceVoice->SetVolume(volume * masterVolume_);
	assert(SUCCEEDED(hr));
	hr = audio.pSourceVoice->Start();
	assert(SUCCEEDED(hr));

	playingSourceList_.push_back(audio.pSourceVoice);
}

/// <summary>
/// Stopの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::StopAudio(IXAudio2SourceVoice* pSourceVoice) {
	pSourceVoice->Stop();
	pSourceVoice->FlushSourceBuffers();
}

/// <summary>
/// soundの一時停止
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::PauseAudio(IXAudio2SourceVoice* pSourceVoice) {
	HRESULT result = pSourceVoice->Stop(0);
	assert(SUCCEEDED(result));
}

/// <summary>
/// 一時停止の再開
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::ReStartAudio(IXAudio2SourceVoice* pSourceVoice) {
	HRESULT result = pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

/// <summary>
/// 
/// </summary>
/// <param name="pSourceVoice"></param>
void Audio::SetVolume(IXAudio2SourceVoice* pSourceVoice, float volume) {
	HRESULT result = S_FALSE;
	result = pSourceVoice->SetVolume(volume);
	assert(SUCCEEDED(result));
}

/// <summary>
/// IsPlayingの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
bool Audio::IsPlaying(IXAudio2SourceVoice* pSourceVoice) {
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	if (state.BuffersQueued == 1) {
		return true;
	} else {
		return false;
	}
}

const char* Audio::GetFileExtension(const char* filename) {
	const char* ext = std::strrchr(filename, '.'); // 最後のピリオドを探す
	if (ext == nullptr) {
		return ""; // 拡張子がない場合は空文字を返す
	}
	return ext + 1; // ピリオドの次の文字から拡張子を返す
}
