#pragma once
#include "../Engine/audio/Audio.h"
#include "../../Engine/light/LightGroup.h"

#include <string>

class GameManager
{
public:
	//������
	void Initialize();
	//�㏈��
	void Finalize();

	//�R���{���Z
	void ComboIncrement();
	//�R���{���Z�b�g
	void ComboReset();
	//�R���{���擾
	int GetComboNum()	{return comboNum;}

	//�I�[�f�B�I�ǉ�
	void AudioAdd(int number, std::string path);
	//�I�[�f�B�I�Đ�
	void AudioPlay(int number, float volume = 1.f, bool loop = false);

	//���C�g�X�V
	void LightUpdate();
	//�v���C���[�ۉe�̐ݒu
	void PlayerCircleShadowSet(Vector3 pos);

private:
	//�I�[�f�B�I������
	void AudioInitialize();

	//���C�g������
	void LightInitialize();

private:
	//�R���{
	int comboNum = 0;

	//�I�[�f�B�I
	Audio* audio = nullptr;

	//���C�g
	LightGroup* lightGroup = nullptr;
	//�ۉe
	//�v���C���[
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f, 0.5f };
};

