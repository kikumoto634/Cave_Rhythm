#pragma once
#include "../BaseObjObject.h"

class Enemy : public BaseObjObject
{
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
	/// �㏈��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

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
};