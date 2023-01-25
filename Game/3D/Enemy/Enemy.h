#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class Enemy : public BaseObjObject
{
//�萔
private:
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 4;
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 50;

//�����o�ϐ�
public:
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//�o��
	void BeginAppearance();
	//�o���\��n�_�Z�b�g
	void SetPopPoasition(Vector3 pos)	{popPosition = pos;}

	//Getter
	bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}
	bool GetIsNotApp()	{return IsNotApp;}
	Vector3 GetDeadParticlePos()	{return DeadParticlePos;}

	//Setter
	void SetDirection(Vector3 _dir);

private:
	//Reset
	void Reset();

	//�o���\��G�t�F�N�g
	void PopParticleApp();
	//���S�G�t�F�N�g
	void DeadParticleApp();

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�����x�N�g��
	Vector3 fallV;

	//���o��
	bool IsNotApp = true;
	//���S��̃��Z�b�g����
	int appearanceResetFrame = 0;

	//���S
	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//���S�n�_(�p�[�e�B�N���p)
	Vector3 DeadParticlePos = {};
	bool IsDeadParticleOnce = false;
	//���S���A�G�ۑ��n�_(�y�ʉ��p)
	Vector3 DeadPos = {50,50,50};

	//�X�P�[��
	bool IsScale = false;

	//�ҋ@
	bool IsWait = false;

	//�ړ�����
	Vector3 direction = {-1,0,0};

	//�|�b�v
	bool IsPop = false;
	int popCount = 0;
	Vector3 popPosition = {};

	//�p�[�e�B�N��
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};