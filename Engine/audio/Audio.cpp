#include "Audio.h"
#include <assert.h>
//�t�@�C���ǂݍ���
#include <fstream>

#pragma comment(lib, "xaudio2.lib")

Audio::~Audio()
{
	masterVoice->DestroyVoice();
	//xAudio2->Release();
	//�ǂݍ��ݍς݃T�E���h�̔g�`�f�[�^�����
	for(auto& pair : soundDatas)
	{
		delete pair.second.pBuffer;
	}
}

void Audio::Initialize()
{
	HRESULT result;

	//XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void Audio::LoadWave(int number, const char *filename)
{
	SoundData soundData{};

	////�t�@�C���I�[�v��
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i���[���[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	////.wav�f�[�^�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff{};
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if(strncmp(riff.chunk.id, "RIFF",4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if(strncmp(riff.type, "WAVE",4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if(strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}
	//�ǂݍ��񂾉����f�[�^��return
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	//Data�`�����N�ǂݍ���
	ChunkHeader data{};
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if(strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݍ���1��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data,sizeof(data));
	}
	if(strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}
	//�g�`�f�[�^�̃T�C�Y�L�^
	soundData.dataSize = data.size;

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	soundData.pBuffer =new char[data.size];
	file.read(soundData.pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&soundData.wfex, &format.fmt, sizeof(format.fmt));
	soundData.wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;


	//�A�z�z��ɗv�f��ǉ�
	soundDatas.insert(std::make_pair(number, soundData));
}

void Audio::PlayWave(int number, float volume, bool IsLoop)
{
	SoundData& soundData = soundDatas[number];

	HRESULT result = S_FALSE;

	//�g�`�t�H�[�}�b�g�����Ƃ�SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice= nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, nullptr);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.dataSize;

	//���[�v
	if(IsLoop){
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->SetVolume(volume);
	result = pSourceVoice->Start();
}

