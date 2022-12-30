#pragma once
#include "../BaseObjObject.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"

#include "PlayerWeapon.h"

class Player : public BaseObjObject
{
//�����o�֐�
public:
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	//Getter
	inline int GetHP()	{return HP;}

	//Setter
	inline void SetHP(int _HP)	{HP = _HP;}

private:
	//�ړ�
	bool Movement();

	//�U��
	bool Attack();

	//�_���[�W
	void Damage();
	//�_���[�W�X�V
	void DamageUpdate();

	//�d��
	void GravityFall();

	//�R���C�_�[
	//�n�ʐڐG����
	void GroundCollider();

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�����x�N�g��
	Vector3 fallV;
	
	//HP
	int HP = 5;

	//�_���[�W
	bool IsDamage = false;
	//���G����(�t���[��)
	const int DamageFrame = 120;
	int damageCurrentFrame = 0;

	//���̓r�[�g
	bool IsInputBeat = false;

	//����
	Input* input = nullptr;

	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;

	//����
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,1,2.5};
};

