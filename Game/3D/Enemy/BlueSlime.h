#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
class BlueSlime : public BaseObjObject
{
//�萔
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 50;

public:
	~BlueSlime();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}

private:
	//���S�G�t�F�N�g
	void DeadParticleApp();

private:
	//����
	bool IsAlive = false;
	Vector3 IsNotAlivePos = {50,50,50};

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
	ParticleObject* DeadParticle = nullptr;
};

