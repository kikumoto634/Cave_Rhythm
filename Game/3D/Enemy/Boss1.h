#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
class Boss1 : public BaseObjObject
{
//�萔
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 6;
	//���S��̃��Z�b�g����
	const int AppDeadParMaxFrame = 150;

	//�����G�t�F�N�g
	const int AppSummonParMaxFrame = 50;
	const int SummonEnemyPosNumMax = 10;

	//�̗�
	const int FullHP = 10;
	//���G����
	const int DamageResetFrame = 10;

	//�ړ��ҋ@
	const int MoveWaitCount = 2;

	//�G�s��
	/*
	* �r�[�g���ɉ����đJ��
	* 1���� 5beat��		����
	* 2�ҋ@ 8beat		�ҋ@
	* 3�ڋ� 2�ҋ@�I���� Reset
	* 4�߂� 3����->���ɖ߂�
	*/

public:
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void ParticleUpdate();
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);
	void IsSummonEnemyPopNot()	{IsSummonEnemyPop = false;}

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline bool GetIsDead()	{return IsDead;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}
	inline bool GetIsSummonEnemyPop()	{return IsSummonEnemyPop;}

private:

	void Movement();
	void Summon();

	//���S�G�t�F�N�g
	void DeadParticleApp();
	//�����G�t�F�N�g
	void SummonParticleApp();

private:

	//���o��
	bool IsNotApp = false;

	//�p�^�[���J�E���g
	int patternCount = 0;

	//HP
	int hp = 0;

	//�_���[�W
	bool IsDamage = false;
	int damageResetCurFrame = 0;

	//�ǐ�
	bool IsMove = true;
	Vector3 targetPos;
	bool IsMoveEasing = false;
	int moveWaitCurCount = 0;
	Vector3 currentPos;
	Vector3 movePosition;
	Vector3 OldPosition = {};
	const float MoveEasingMaxTime = 0.05f;
	float moveEasingFrame = 0;

	//����
	bool IsSummon = false;
	bool IsSummonEnemyPop = false;
	int summonEnemyPosNum = 0;

	//�ދp
	bool IsComeBack= false;
	Vector3 homePos = {};

	//�R���C�_�[
	float radius = 1.0f;

	Vector3 NotAlivePos = {50,50,50};
	//���S
	bool IsDead = false;
	bool IsDeadOnceAudio = false;

	//�����ɉ�������\��
	bool IsInvisible = false;
	float distance = 0.f;

	//�X�P�[��
	bool IsScaleEasing = false;

	//�p�[�e�B�N��
	std::unique_ptr<ParticleObject> DeadParticle;
	int appDeadParFrame = 0;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	std::unique_ptr<ParticleObject> SummonParticle;
	int appSummonParFrame = 0;
	Vector3 SummonParticlePos ={};
};

