#include "Player.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

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
	//���S���f��
	deadModel = new ObjModelManager();
	deadModel->CreateModel("human3");

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 1.0f, 0.7f};

	//�ҋ@�t���O
	IsWait = true;

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

	CameraTarget= camera->GetTarget();
	CameraEye = camera->GetEye();

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

			//�ҋ@�t���O����
			if(IsWait && !IsNextScene){
				IsWait = false;
			}
		}

		if(IsInputJudge){
			IsInputJudge = false;
			//�ړ�
			if(IsMove){
				IsMoveEasing = true;
				IsMoveCameraEasing = true;
				IsMove = false;
				world.rotation = moveRotation;
				moveEasingPos = world.translation;
				MoveModelSet();
			}
			//�U��
			else if(IsAttack){
				weapon->Attack();
				AttackModelSet();
				IsAttack = false;
			}
		}
	}

	//�ړ��C�[�W���O
	if(IsMoveEasing){
		world.translation = Easing_Linear_Point2(moveEasingPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));
		//camera->MoveVector({moveCameraPosition.x/5, 0, moveCameraPosition.z/5});

		if(moveEasingFrame >= 1.f){
			IsMoveEasing = false;
			world.translation = movePosition;
			moveEasingPos = {};
			movePosition = {};
			moveEasingFrame = 0;
		}
	}
	if(IsMoveCameraEasing){
		CameraCurrentPosition = Easing_Linear_Point2({0,0,0},moveCameraPosition/2, Time_OneWay(moveEasingCameraFrame, MoveEasingCameraMaxTime));
		camera->MoveVector(CameraCurrentPosition);

		if(moveEasingCameraFrame >= 1.f){
			IsMoveCameraEasing = false;
			moveCameraPosition = {};
			CameraCurrentPosition = {};
			moveEasingCameraFrame = 0;
		}
	}

	//���S
#ifdef _DEBUG
	if(input->Trigger(DIK_SPACE)){
		HP = 0;
	}
#endif // _DEBUG
	if(HP <= 0 && !IsDead){
		IsDead = true;
		IsDeadAudioOnce = true;
		this->object->SetModel(deadModel);
		this->camera->ShakeStart();
	}

	//�ړ�����
	world.translation.x = max(world.translation.x , 12 * -2.f);
	world.translation.x = min(world.translation.x , 12 * 2.f);
	world.translation.z = max(world.translation.z , 12 * -2.f);
	world.translation.z = min(world.translation.z , 12 * 2.f);

	this->camera->view.target.x = min(this->camera->view.target.x, 24.f);
	this->camera->view.target.x = max(this->camera->view.target.x, -24.f);
	this->camera->view.target.z = min(this->camera->view.target.z, 19.f);
	this->camera->view.target.z = max(this->camera->view.target.z, -29.f);

	this->camera->view.eye.x = min(this->camera->view.eye.x, 24.f);
	this->camera->view.eye.x = max(this->camera->view.eye.x, -24.f);
	this->camera->view.eye.z = min(this->camera->view.eye.z, 10.f);
	this->camera->view.eye.z = max(this->camera->view.eye.z, -38.f);

	//����ʒu
	weapon->SetPosition(world.translation + offSetWeaponPos);
	weapon->SetRotation(GetRotation());

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

	delete deadModel;
	deadModel = nullptr;

	delete attackModel;
	attackModel = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	if(IsWait) return;
	if(IsDead) return;

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

bool Player::GetIsDeadAudioOnce()
{
	if(IsDeadAudioOnce) {
		IsDeadAudioOnce = false;
		return true;
	}

	return false;
}


bool Player::MovementInput()
{
	//�߂�l
	bool IsReturn = false;

	if(IsWait) return false;
	if(IsDead) return false;
	if(IsAttack) return false;
	if(IsMoveEasing) return false;

	//���s
	if(input->Trigger(DIK_UP)){
		movePosition = Vector3{0.0f,0.0f,2.f};
		moveRotation.y = 0;
		offSetWeaponPos = {0,0,2.f};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_DOWN)){
		movePosition = Vector3{0.0f,0.0f,-2.f};
		moveRotation.y = XMConvertToRadians(180);
		offSetWeaponPos = {0,0,-2.f};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_RIGHT)){
		movePosition = Vector3{2.f,0.0f,0.0f};
		moveRotation.y = XMConvertToRadians(90);
		offSetWeaponPos = {2.f,0,0};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_LEFT)){
		movePosition = Vector3{-2.f,0.0f,0.0f};
		moveRotation.y = XMConvertToRadians(-90);
		offSetWeaponPos = {-2.f,0,0};
		IsReturn = true;
	}
	if(IsReturn) {
		//���f������
		IsModelJudge = false;
		//�s��
		IsMove = true;
		//�ړ�����W
		moveCameraPosition = movePosition;
		movePosition += world.translation;
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

	if(IsWait) return false;
	if(IsDead) return false;
	if(IsMove) return false;

	if(input->Trigger(DIK_Z)){
		IsReturn = true;

		//�o��
		if(IsExitOpen){
			IsNextScene = true;
			return false;
		}
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
