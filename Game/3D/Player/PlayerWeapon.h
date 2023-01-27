#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/SphereCollider.h"

class PlayerWeapon : public BaseObjObject
{
//�����o�֐�
public:
	~PlayerWeapon();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision(const CollisionInfo &info) override;

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();


private:
	//����o��
	bool IsAppear = false;

	//��������(�t���[��)
	float aliveCurrentTime = 0;
	const float AliveTime = 0.2f;

	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;
};

