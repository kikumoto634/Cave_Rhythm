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

public:
	~Boss1();
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

	//���S�G�t�F�N�g
	void DeadParticleApp();

private:

	//���o��
	bool IsNotApp = false;

	Vector3 NotAlivePos = {50,50,50};
	//���S
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//�R���C�_�[
	bool IsCollision = false;

	//����
	int appearancePopFrame = 0;

	//�����ɉ�������\��
	bool IsInvisible = false;
	float distance = 0.f;

	//�X�P�[��
	bool IsScaleEasing = false;

	//�p�[�e�B�N��
	ParticleObject* DeadParticle = nullptr;
};

