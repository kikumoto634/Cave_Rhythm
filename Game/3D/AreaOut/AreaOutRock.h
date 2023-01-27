#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class AreaOutRock : public BaseObjObject
{
private:
	const float LowestPoint = -30.f;

public:
	~AreaOutRock();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D�`��
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

private:
	//�d��
	void GravityFall();

	//�����_���X�|�[��
	void RandomSpawn();

	//���X�|�[���^�C��
	void ReSpawnTime();

	//�X�P�[�������_��
	void ScaleRandom();

	void CreateParticle();

private:
	//�����x�N�g��
	Vector3 fallV;

	//����
	bool IsAlive = false;

	//�X�|�[���t���[��
	int spawnFame = 200;
	int MaxSpawnFrame = 300;

	//�p�[�e�B�N��
	ParticleObject* PopParticle = nullptr;
};

