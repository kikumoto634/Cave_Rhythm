#pragma once
#include <xaudio2.h>
#include <wrl.h>

class Audio
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//チャンクヘッダ
	struct ChunkHeader{
		char id[4];
		int32_t size;
	};

	//RIFFヘッダチャンク
	struct RiffHeader{
		ChunkHeader chunk;
		char type[4];
	};

	//FMIチャンク
	struct FormatChunk{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	//音声データ
	struct SoundData{
		//波形フォーマット
		WAVEFORMATEX  wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:
	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* fileName);

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};

