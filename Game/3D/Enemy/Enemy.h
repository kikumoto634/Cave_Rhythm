#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class Enemy : public BaseObjObject
{
//�萔
private:
	//�r�[�gx�񐔏I������POP
	const int POP_COUNT = 3;

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

	//�o���\��G�t�F�N�g
	void PopParticleApp();

	//Getter

	//Setter
	void SetDirection(Vector3 _dir);

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�����x�N�g��
	Vector3 fallV;
	//���S
	bool IsDead = false;
	//�X�P�[��
	bool IsScale = false;

	//�ړ�����
	Vector3 direction = {-1,0,0};

	//���S���A�G�ۑ��n�_
	Vector3 DeadPos = {50,50,50};

	//�|�b�v
	bool IsPop = false;
	int popCount = 0;
	ParticleObject* PopParticle = nullptr;
};