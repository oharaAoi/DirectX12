#include "Audio.h"

Audio::~Audio() {
	xAudio2_.Reset();
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

void Audio::Finalize() {

}

/// <summary>
/// 音源のデータを読む
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
LoadData Audio::LoadWave(const char* filename) {
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

	LoadData loadData;
	loadData.fmt = format.fmt;
	loadData.pBuffer = pBuffer;
	loadData.dataSize = data.size;

	return loadData;
}


/// <summary>
/// 音声データの読み込み
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
SoundData Audio::SoundLoadWave(const char* filename) {
	LoadData loadData = LoadWave(filename);
	// 読み込んだ音声データをreturn
	SoundData soundData = {};
	soundData.wfex = loadData.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(loadData.pBuffer);
	soundData.bufferSize = loadData.dataSize;
	
	return soundData;
}

BgmData Audio::LoadBGM(const char* filename) {
	LoadData loadData = LoadWave(filename);
	// 読み込んだ音声データをreturn
	BgmData bgm = {};
	bgm.data.wfex = loadData.fmt;
	bgm.data.pBuffer = reinterpret_cast<BYTE*>(loadData.pBuffer);
	bgm.data.bufferSize = loadData.dataSize;
	HRESULT result = xAudio2_->CreateSourceVoice(&bgm.pSourceVoice, &bgm.data.wfex);
	assert(SUCCEEDED(result));

	return bgm;
}

SeData Audio::LoadSE(const char* filename) {
	LoadData loadData = LoadWave(filename);
	// 読み込んだ音声データをreturn
	SeData se = {};
	se.data.wfex = loadData.fmt;
	se.data.pBuffer = reinterpret_cast<BYTE*>(loadData.pBuffer);
	se.data.bufferSize = loadData.dataSize;

	return se;
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

	if (!IsPlaying(pSourceVoice)) {
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
	result = pSourceVoice->Start();
}

/// <summary>
/// BGMの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::PlayBGM(const BgmData& soundData, const bool& isLoop) {
	HRESULT result = S_FALSE;

	if (!IsPlaying(soundData.pSourceVoice)) {
		return;
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.data.pBuffer;
	buf.AudioBytes = soundData.data.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	// ループするかしないか
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループ
	} else {
		buf.LoopCount = 0; // ループしない
	}

	// 波形データの再生
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = soundData.pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

/// <summary>
/// SEの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
void Audio::PlaySE(const SeData& soundData, const bool& isLoop) {
	HRESULT result = S_FALSE;
	// 波形フォーマットを元にsourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.data.wfex);
	assert(SUCCEEDED(result));
	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.data.pBuffer;
	buf.AudioBytes = soundData.data.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	// ループするかしないか
	buf.LoopCount = isLoop;
	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
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
/// IsPlayingの再生
/// </summary>
/// <param name="xAudio2"></param>
/// <param name="soundData"></param>
bool Audio::IsPlaying(IXAudio2SourceVoice* pSourceVoice) {
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	return state.BuffersQueued == 0;
}
