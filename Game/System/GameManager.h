#pragma once
#include "../Engine/audio/Audio.h"
#include "../../Engine/light/LightGroup.h"

#include "../Game/2D/BaseSprites.h"

#include <string>
#include <vector>

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
	//�n�ʐF�ω��R���{��
	int GetPlaneColorChangeCombo()	{return PlaneColorChangeConboNum;}

	//�R�C�����Z
	void CoinIncrement();
	//�R�C�����擾
	int GetCoinNum()	{return coinNum;}

	//�̗͏����ݒ�
	void InitializeSetHp(int _hp);
	//�̗̓X�v���C�g���Y���t���O
	void IsBeatEndOn()	{IsHpScaleChange = true;}
	//�̗͌���
	void HpDecrement();

	//�I�[�f�B�I�Đ�
	void AudioPlay(int number, float volume = 1.f, bool loop = false);

	//���C�g�X�V
	void LightUpdate(bool IsPlayerShadowDead = false);
	//�v���C���[�ۉe�̐ݒu
	void PlayerCircleShadowSet(Vector3 pos);

	//�G����
	int EnemyPopTurnCount();
	Vector2 EnemyRandomPos(const int groundWidth, const float Plane_Size);
	Vector2 EnemyRandomDir(Vector2 pos);

	//2D�X�v���C�g�X�V
	void SpriteUpdate();

	//2D�X�v���C�g�`��
	void SpriteDraw();

private:
	//�I�[�f�B�I������
	void AudioInitialize();

	//���C�g������
	void LightInitialize();

	//�X�v���C�g������
	void SpriteInitialize();

private:
	//�R���{
	int comboNum = 0;
	const int PlaneColorChangeConboNum = 5;

	//�R�C��
	int coinNum = 0;

	//�̗�
	int hp = 0;

	//�I�[�f�B�I
	Audio* audio = nullptr;

	//���C�g
	LightGroup* lightGroup = nullptr;
	//�ۉe
	//�v���C���[
	bool IsPlayerShadowAlive = false;
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f, 0.5f };

	//�G�̃����_������
	//X�r�[�g��POP�^�[��
	const int EnemyPopBeatTurn = 6;
	int currentEnemyPopBeatTurn = 0;
	//1�r�[�g�ł̓GPOP��
	const int EnemyPopCreateNum = 1;


	//2D�e�L�X�g
	const int NumberSpSize = 3;
	const int TexNumberBegin = 3;
	
	//�R���{�e�L�X�g
	std::unique_ptr<BaseSprites> comboSp;
	//����(TexNumber 3~12)
	std::unique_ptr<BaseSprites> numberSp_combo[3];

	//�l���R�C��
	std::unique_ptr<BaseSprites> coinSp;
	std::unique_ptr<BaseSprites> numberSp_coin[3];

	//�̗�
	int HpSpSize = 0;
	int DamageHpSpriteIndex = 0;
	std::vector<std::unique_ptr<BaseSprites>> hpSp;
	bool IsHpScaleChange = false;
};

