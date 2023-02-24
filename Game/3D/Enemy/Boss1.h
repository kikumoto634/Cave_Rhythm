#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
class Boss1 : public BaseObjObject
{
//�萔
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 6;
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 50;

	//�̗�
	const int FullHP = 5;
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
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline bool GetIsDead()	{return IsDead;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

private:

	void Movement();

	//���S�G�t�F�N�g
	void DeadParticleApp();

private:

	//���o��
	bool IsNotApp = false;

	//HP
	int hp = 0;

	//�_���[�W
	bool IsDamage = false;
	int damageResetCurFrame = 0;

	//�ǐ�
	Vector3 targetPos;
	bool IsMoveEasing = false;
	int moveWaitCurCount = 0;
	Vector3 currentPos;
	Vector3 movePosition;
	const float MoveEasingMaxTime = 0.05f;
	float moveEasingFrame = 0;

	//�ދp
	bool IsComeBack= false;
	Vector3 originpos = {};

	//�R���C�_�[
	float radius = 1.0f;

	Vector3 NotAlivePos = {50,50,50};
	//���S
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//�����ɉ�������\��
	bool IsInvisible = false;
	float distance = 0.f;

	//�X�P�[��
	bool IsScaleEasing = false;

	//�p�[�e�B�N��
	std::unique_ptr<ParticleObject> DeadParticle;
	int appearancePopFrame = 0;
};

