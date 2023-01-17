#pragma once
#include "../BaseObjObject.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"

#include "PlayerWeapon.h"

//ToDO �v���C���[�̃��t�@�N�^�����O
/*
�����ꗗ
Initialize();
�E�X�P�[���C�[�W���O�̍ŏ��l�ݒ�
�EInput��instance�擾
�E�R���C�_�[�Z�b�g(�����������������R���C�_�[���m�̓����蔻��)
�E�����Z�b�g
�E���R���C�_�[�̎擾(���C���΂��Ă̓����蔻��p)
�E����̎擾(new)
�E�U�����f���̓ǂݍ���(new)

Update();
�E�J�����̎󂯎��

����
�E�{�^�������͂��ꂽ���̊m�F
�E�ړ�(���f���ύX) ���W�A��]�A�ߋ����W�A����̃I�t�Z�b�g���W�̎擾
�E�U��(���f���ύX + weapon->Attack()�Ăяo��)
�E�ړ�����

�X�V
�E�r�[�g������ (�T�C�Y�C�[�W���O)
�E�_���[�W(�ڐG���̖��G���Ԍv��)
�E�d��(�K�v���������Ȃ�)
�E����ʒu�̍X�V
�EBase�̍X�V
�E�R���C�_�[�X�V
�E�n�ʂ����邩�������̎擾(Ray)
�E����X�V

�`��
�E����`��
�E�v���C���[�`��

�ڐG��
�E�G(�_���[�W)

�㏈��
�E����㏈���A���
�E�U�����f�����
�EBase�̉��
*/

class Player : public BaseObjObject
{
//�萔
//���G����
const int DamageFrame = 120;

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

	/// <summary>
	/// ���͐��ێ��̏���
	/// </summary>
	/// <param name="IsFlag">����</param>
	//void JudgeUpdate(bool IsFlag);

	/// <summary>
	/// �_���[�W�T�E���h����
	/// </summary>
	bool DamageSound();

	//Getter
	inline int GetHP()	{return HP;}
	inline bool GetIsInputOnce()	{return IsInputOnce;}

private:
	//�ړ�
	bool MovementInput();
	void MoveModelSet();

	//�U��
	bool AttackInput();
	void AttackModelSet();

	//�_���[�W
	void Damage();
	//�_���[�W�X�V
	void DamageUpdate();

	//�d��
	//void GravityFall();

	//�R���C�_�[
	//�n�ʐڐG����
	//void GroundCollider();

private:
	//�ڒn�t���O
	//bool IsGround = true;
	//�����x�N�g��
	//Vector3 fallV;
	
	//�ړ�
	Vector3 movePosition = {0,0,0};
	Vector3 moveRotation = {0,0,0};
	Vector3 oldPosition = {};

	//HP
	int HP = 5;

	//�_���[�W
	bool IsDamage = false;
	bool IsDamageSoundOnce = false;
	////���G����(�t���[��)
	int damageCurrentFrame = 0;

	//�g�p���f������(false:Move, true:Attack)
	bool IsModelJudge = false;

	//����
	Input* input = nullptr;
	bool IsInputOnce = false;

	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;

	//����
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,0,2.5};

	//�U�����f��
	ObjModelManager* attackModel = nullptr;
};

