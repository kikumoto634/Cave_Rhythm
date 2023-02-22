#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
#include "../../Collision/SphereCollider.h"

class Skelton : public BaseObjObject
{
//�萔
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 6;
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 50;
	//�ړ��ҋ@
	const int WaitCount = 2;

public:
	~Skelton();
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
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

private:
	void Reset();

	void Movement();

	//�o���\��G�t�F�N�g
	void PopParticleApp();
	//���S�G�t�F�N�g
	void DeadParticleApp();

private:

	//�ҋ@����
	int waitCount = 0;

	//���C
	Vector3 RayDir = {0,0,1};
	Vector3 OldPosition = {};
	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;
	bool IsCollision = true;

	//���o��
	bool IsNotApp = false;

	Vector3 NotAlivePos = {50,50,50};
	//���S
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//����
	int appearancePopFrame = 0;

	//�|�b�v
	bool IsPop = false;
	int popCount = 0;
	Vector3 PopParticlePos = {};

	//�����ɉ�������\��
	bool IsInvisible = false;
	float distance = 0.f;

	//�ǐ�
	Vector3 targetPos;
	bool IsMoveEasing = false;
	Vector3 currentPos;
	Vector3 movePosition;
	const float MoveEasingMaxTime = 0.05f;
	float moveEasingFrame = 0;

	//�X�P�[��
	bool IsScaleEasing = false;

	//�p�[�e�B�N��
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};

