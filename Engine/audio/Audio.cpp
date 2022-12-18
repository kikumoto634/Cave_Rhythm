#include "Audio.h"

#include <fstream>
#include <cassert>

#pragma comment(lib, "xaudio2.lib")

void Audio::Initialize()
{
	HRESULT result;

	//XAudio�̃G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

SoundData Audio::SoundLoadWave(const char *fileName)
{
	return SoundData();
}
