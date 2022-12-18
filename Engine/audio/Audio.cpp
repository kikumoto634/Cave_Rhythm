#include "Audio.h"

#include <fstream>
#include <cassert>

#pragma comment(lib, "xaudio2.lib")

void Audio::Initialize()
{
	HRESULT result;

	//XAudioのエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

SoundData Audio::SoundLoadWave(const char *fileName)
{
	return SoundData();
}
