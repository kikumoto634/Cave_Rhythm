#include "Player.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;


Player::~Player()
{
	//Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//����
	input = Input::GetInstance();
	//����
	weapon = new PlayerWeapon();
	weapon->Initialize("Slash", true);
	//�U�����f��
	attackModel = new ObjModelManager();
	attackModel->CreateModel("human2");

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,-0.2f,0,0}, radius));
	//�����蔻�葮��
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	//���R���C�_�[�擾
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;
	
	//Once�p�����[�v������
	IsInputOnce = false;
	IsDamageSoundOnce = false;

	//���͏���
	if(MovementInput()){
		//���͊m�F
		IsInputOnce = true;
	}
	else if(AttackInput()){
		//���͊m�F
		IsInputOnce = true;
	}

	//���I��(�Q�[���S�̂̎��Ԃ𑪂��Ă���N���X(GameScene)����IsBeatEndOn()���Ăяo����true���擾)
	if(IsBeatEnd){
		//�X�P�[���C�[�W���O
		if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
			IsBeatEnd = false;
		}

		if(IsInputJudge){
			IsInputJudge = false;
			//�ړ�
			if(IsMove){
				world.translation += movePosition;
				world.rotation = moveRotation;
				MoveModelSet();
				movePosition = {};
				IsMove = false;
			}
			//�U��
			else if(IsAttack){
				weapon->Attack();
				AttackModelSet();
				IsAttack = false;
			}
		}
	}

	//�ړ�����
	world.translation.x = max(world.translation.x , -12.5f);
	world.translation.x = min(world.translation.x , 12.5f);
	world.translation.z = max(world.translation.z , -12.5f);
	world.translation.z = min(world.translation.z , 12.5f);

	//����ʒu
	weapon->SetPosition(world.translation + offSetWeaponPos);
	weapon->SetRotation(moveRotation);

	//�X�V�֐�
	//�_���[�W
	DamageUpdate();
	//�R���C�_�[�X�V
	collider->Update();
	//����
	weapon->Update(this->camera);
	//�x�[�X�X�V
	BaseObjObject::Update(this->camera);
}

void Player::Draw()
{
	weapon->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	weapon->Finalize();
	delete weapon;
	weapon = nullptr;

	delete attackModel;
	attackModel = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	//�G�ڐG
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		Damage();
		camera->ShakeStart();
	}
}


void Player::JudgeUpdate(bool IsFlag)
{
	//��
	if(IsFlag){
		IsInputJudge = true;
		return;
	}
	//��
	IsInputJudge = false;
	return;
}

bool Player::DamageSound()
{
	if(IsDamage && IsDamageSoundOnce) return true;
	return false;
}



bool Player::MovementInput()
{
	//�߂�l
	bool IsReturn = false;

	if(IsAttack) return false;

	//�ߋ��̈ʒu�擾
	oldPosition = GetPosition();
	//���s
	if(input->Trigger(DIK_UP)){
		movePosition.z = 2.5f;
		moveRotation.y = 0;
		offSetWeaponPos = {0,0,2.5};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_DOWN)){
		movePosition.z = -2.5f;
		moveRotation.y = XMConvertToRadians(180);
		offSetWeaponPos = {0,0,-2.5};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_RIGHT)){
		movePosition.x = 2.5f;
		moveRotation.y = XMConvertToRadians(90);
		offSetWeaponPos = {2.5,0,0};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_LEFT)){
		movePosition.x = -2.5f;
		moveRotation.y = XMConvertToRadians(-90);
		offSetWeaponPos = {-2.5,0,0};
		IsReturn = true;
	}
	if(IsReturn) {
		//���f������
		IsModelJudge = false;
		//�s��
		IsMove = true;
	}
	return IsReturn;
}

void Player::MoveModelSet()
{
	this->object->SetModel(model);
}

bool Player::AttackInput()
{
	//�߂�l
	bool IsReturn = false;

	if(IsMove) return false;

	if(input->Trigger(DIK_Z)){
		IsReturn = true;
	}

	if(IsReturn) {
		//���f������
		IsModelJudge = true;
		//�s��
		IsAttack = true;
	}
	return IsReturn;
}

void Player::AttackModelSet()
{
	this->object->SetModel(attackModel);
}

void Player::Damage()
{
	if(IsDamage) return;
	IsDamage = true;
	IsDamageSoundOnce = true;
	HP -= 1;
}

void Player::DamageUpdate()
{
	if(!IsDamage) return ;

	damageCurrentFrame += 1;

	//���G���ԓ�
	Vector4 color = (damageCurrentFrame % 6 == 1) ? color = {0.0f, 0.0f, 0.0f, 0.0f} : color = {1.0f, 0.0f, 0.0f, 1.0f};
	object->SetColor(color);

	//���G���ԏI��
	if(damageCurrentFrame < DamageFrame) return;
	damageCurrentFrame = 0;
	object->SetColor({1.0f,1.0f,1.0f, 1.0f});
	IsDamage = false;
}
