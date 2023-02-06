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
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw() override;

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
	void JudgeUpdate(bool IsFlag);


	//Sound
	/// <summary>
	/// �_���[�W�T�E���h����
	/// </summary>
	bool DamageSound();

	bool GetIsDead()	{return IsDead;};
	bool GetIsDeadAudioOnce();


	//Getter
	inline int GetHP()	{return HP;}
	inline bool GetIsInputOnce()	{return IsInputOnce;}
	inline bool GetIsNextScene()	{return IsNextScene;}

	//Setter
	inline void SetMoveEasingMaxTime(float time)	{MoveEasingMaxTime = time;}
	inline void SetIsExitOpen(bool IsFlag)	{IsExitOpen = IsFlag;}
	inline void SetIsWait(bool IsFlag)	{IsWait = IsFlag;}
	inline void SetWeaponPos(Vector3 pos)	{offSetWeaponPos = pos;}
	inline void SetHp(int hp)	{HP = hp;}
	inline void SetCameeraInitPos(Vector3 pos)	{InitializeCameraPos = pos;}

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

private:	
	//�ړ�
	Vector3 movePosition = {0,0,0};
	Vector3 moveRotation = {0,0,0};
	bool IsMove = false;

	//�ߋ��ʒu
	Vector3 OldPosition = {};
	Vector3 OldCameraPosition = {};

	//�J�����ړ�
	bool IsCameraInit = false;
	Vector3 moveCameraPosition = {0,0,0};
	Vector3 InitializeCameraPos = {};

	//�ړ��C�[�W���O
	bool IsMoveEasing = false;
	float moveEasingFrame = 0;
	float MoveEasingMaxTime = 0.05f;
	Vector3 moveEasingPos;

	bool IsMoveCameraEasing = false;
	float moveEasingCameraFrame = 0;
	float MoveEasingCameraMaxTime = 0.05f;
	Vector3 CameraCurrentPosition = {0,0,0};

	//���C
	Vector3 RayDir = {0,0,1};

	//HP
	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	int HP = 5;

	//�ҋ@�t���O
	bool IsWait = false;

	//�_���[�W
	bool IsDamage = false;
	bool IsDamageSoundOnce = false;
	////���G����(�t���[��)
	int damageCurrentFrame = 0;

	//�U��
	bool IsAttack = false;

	//�g�p���f������(false:Move, true:Attack)
	bool IsModelJudge = false;

	//����
	Input* input = nullptr;
	bool IsInputOnce = false;
	bool IsInputJudge = false;

	//�R���C�_�[
	SphereCollider* sphereCollider = nullptr;

	//����
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,0,2.5};

	//�U�����f��
	ObjModelManager* attackModel = nullptr;

	//���S���f��
	ObjModelManager* deadModel = nullptr;

	//���̃V�[��
	bool IsNextScene = false;
	bool IsExitOpen = false;
};

