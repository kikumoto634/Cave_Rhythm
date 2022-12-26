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

	//���S���A�G�ۑ��n�_
	Vector3 DeadPos = {50,50,50};
};