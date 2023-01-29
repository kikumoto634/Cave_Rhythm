#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class Enemy2 : public BaseObjObject
{
//�萔
private:
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 6;
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 50;
	//�ҋ@�e���|��
	const int WaitTime = 4;

//�����o�ϐ�
public:
	~Enemy2();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera, Vector3 targetPos);

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
	inline void SetMoveEasingMaxTime(float time)	{MoveEasingMaxTime = time;}


private:
	//Reset
	void Reset();

	//�o���\��G�t�F�N�g
	void PopParticleApp();
	//���S�G�t�F�N�g
	void DeadParticleApp();

	//�^�[�Q�b�g�ړ�
	void TargetMove(Vector3 targetPos);

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
	bool IsDead = true;
	bool IsDeadAudioOnce = false;
	//���S�n�_(�p�[�e�B�N���p)
	Vector3 DeadParticlePos = {};
	bool IsDeadParticleOnce = false;
	//���S���A�G�ۑ��n�_(�y�ʉ��p)
	Vector3 DeadPos = {50,50,50};

	//�X�P�[��
	bool IsScale = false;

	//�ҋ@
	int waitNum = 0;

	//�ړ�����
	Vector3 direction = {-1,0,0};

	//�ړ�
	Vector3 movePosition;
	bool IsMoveEasing = false;
	float moveEasingFrame = 0;
	float MoveEasingMaxTime = 0.05f;
	Vector3 moveEasingPos;


	//�|�b�v
	bool IsPop = false;
	int popCount = 0;
	Vector3 popPosition = {};

	//�p�[�e�B�N��
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};