#include "Player.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

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

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));

	//�����蔻�葮��
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;

	//����
	Input* input = Input::GetInstance();
	if(input->Trigger(DIK_UP)){
		world.translation.z += 2.f;
		world.rotation.y = 0;
	}
	else if(input->Trigger(DIK_DOWN)){
		world.translation.z -= 2.f;
		world.rotation.y = XMConvertToRadians(180);
	}
	else if(input->Trigger(DIK_RIGHT)){
		world.translation.x += 2.f;
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(input->Trigger(DIK_LEFT)){
		world.translation.x -= 2.f;
		world.rotation.y = XMConvertToRadians(-90);
	}

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
	//�W�����v����
	else if(input->Trigger(DIK_SPACE)){
		IsGround = false;
		const float jumpVYFist = 0.2f;	//�W�����v�����������
		fallV = {0,jumpVYFist, 0};
	}

	//�s��A�J�����X�V
	BaseObjObject::Update(this->camera);
	//�R���C�_�[�X�V
	collider->Update();

	//���R���C�_�[�擾
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);


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

	BaseObjObject::Update(this->camera);
}

void Player::Draw3D()
{
	BaseObjObject::Draw();
}


void Player::Finalize()
{
	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{

}

