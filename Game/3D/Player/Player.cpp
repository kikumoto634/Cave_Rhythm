#include "Player.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;


Player::~Player()
{
	Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//����
	input = Input::GetInstance();

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));

	//�����蔻�葮��
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//���R���C�_�[�擾
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//����
	weapon = new PlayerWeapon();
	weapon->Initialize("sphere", true);
	weapon->GetmatWorld().parent = &world;
}

void Player::Update(Camera *camera)
{
	this->camera = camera;
	
	//�ړ�
	Movement();

	if(input->Trigger(DIK_RETURN)){
		weapon->Attack();
	}

	//���I��
	if(IsBeatEnd){
		//�T�C�Y�ύX
		if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
			IsBeatEnd = false;
		}
	}

	//�_���[�W
	DamageUpdate();

	//�d��
	GravityFall();

	//�s��A�J�����X�V
	BaseObjObject::Update(this->camera);
	//�R���C�_�[�X�V
	collider->Update();

	//�n�ʐڐG����
	GroundCollider();

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

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	//�G�ڐG
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		Damage();
	}
}


void Player::Movement()
{
	//���s
	if(input->Trigger(DIK_UP)){
		world.translation.z += 2.5f;
		world.rotation.y = 0;
	}
	else if(input->Trigger(DIK_DOWN)){
		world.translation.z -= 2.5f;
		world.rotation.y = XMConvertToRadians(180);
	}
	else if(input->Trigger(DIK_RIGHT)){
		world.translation.x += 2.5f;
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(input->Trigger(DIK_LEFT)){
		world.translation.x -= 2.5f;
		world.rotation.y = XMConvertToRadians(-90);
	}
}

void Player::Damage()
{
	if(IsDamage) return;
	IsDamage = true;
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

void Player::GravityFall()
{
	//��������
	if(!IsGround){
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallV.y = max(fallV.y + fallAcc, fallVYMin);
		//�ړ�
		world.translation.x += fallV.x;
		world.translation.y += fallV.y;
		world.translation.z += fallV.z;
	}
}

void Player::GroundCollider()
{
	//���̏�[���狅�̉��[�܂ł̗�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = {0,-1,0,0};
	RaycastHit raycastHit;

	//�ڒn���
	if(IsGround){
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		//�ڒn���ێ�
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
			IsGround = true;
			world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
			//�s��̍X�V�Ȃ�
			BaseObjObject::Update(this->camera);
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else{
			IsGround = false;
			fallV = {};
		}
	}
	//�������
	else if(fallV.y <= 0.0f){
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius()*2.0f)){
			//���n
			IsGround = true;
			world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
			//�s��̍X�V�Ȃ�
			BaseObjObject::Update(this->camera);
		}
	}
}

