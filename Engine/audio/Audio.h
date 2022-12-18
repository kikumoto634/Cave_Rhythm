#pragma once
#include <xaudio2.h>
#include <wrl.h>

class Audio
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�`�����N�w�b�_
	struct ChunkHeader{
		char id[4];
		int32_t size;
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader{
		ChunkHeader chunk;
		char type[4];
	};

	//FMI�`�����N
	struct FormatChunk{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	//�����f�[�^
	struct SoundData{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX  wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
	};

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

private:
	/// <summary>
	/// �����f�[�^�ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* fileName);

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};

