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
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision(const CollisionInfo &info);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:
	//����o��
	bool IsAppear = false;

	//��������(�t���[��)
	int aliveCurrentFrame = 0;
	const int AliveFrame = 10;

	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;
};

