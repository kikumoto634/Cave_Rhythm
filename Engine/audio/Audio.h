#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <map>

/// <summary>
/// �T�E���h�Đ��Ǘ�
/// </summary>
class Audio
{
public://1�G�C���A�X
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template<class T1, class T2> using map = std::map<T1,T2>;

public://�C���i�[�N���X
	//�`�����N�w�b�_�[
	struct ChunkHeader
	{
		char id[4];//�`�����N����ID
		int size;	//�`�����N�T�C�Y
	};
	//RIFF�w�b�_�`�F�b�N
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];	//"WAVE"
	};
	//FMT�`�F�b�N
	struct FormatChunk
	{
		ChunkHeader chunk;	//"fmt"
		WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex{};
		//�g�`�f�[�^
		char* pBuffer;
		//�g�`�f�[�^�̃T�C�Y
		unsigned int dataSize;
	};

public://�����o�֐�

	/// <summary>
	///�f�X�g���N�^
	/// </summary>
	~Audio();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �T�E���h�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="number">�T�E���h�ԍ�</param>
	/// <param name="filename">wav�t�@�C����</param>
	void LoadWave(int number, const char* filename);

	//�T�E���h�Đ�
	void PlayWave(int number, float volume = 1.0f, bool IsLoop = false);

private://�����o�ϐ�
	//XAudio2�̃C���X�^���X
	ComPtr<IXAudio2> xAudio2;
	//�}�X�^�[�{�C�X
	IXAudio2MasteringVoice* masterVoice;
	//�g�`�f�[�^�̘A�z�z��
	map<int, SoundData> soundDatas;
};

