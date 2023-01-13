#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class Enemy : public BaseObjObject
{
//�萔
private:
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 4;

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
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//�o���\��n�_�Z�b�g
	void SetPopPoasition(Vector3 pos)	{popPosition = pos;}
	//�o���\��G�t�F�N�g
	void PopParticleApp();
	//���S�G�t�F�N�g
	void DeadParticleApp();

	//Getter
	bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}

	//Setter
	void SetDirection(Vector3 _dir);

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�����x�N�g��
	Vector3 fallV;

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
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};