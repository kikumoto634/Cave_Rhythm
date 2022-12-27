#include "Enemy.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#ifdef _DEBUG
	#include "../../../Engine/input/Input.h"
#endif // _DEBUG


Enemy::~Enemy()
{
	Finalize();
}

void Enemy::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//��]
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);

	//�R���C�_�[�ǉ�
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void Enemy::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsDead){

		//���I��
		if(IsBeatEnd){

			//�ړ�
			world.translation += direction*2.5f;

			//�X�P�[��
			IsScale  = true;

			IsBeatEnd = false;
		}

		//�X�P�[���J��
		if(IsScale){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScale = false;
			}
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
	}

	BaseObjObject::Update(this->camera);
}

void Enemy::Draw()
{
	if(IsDead) return;

	BaseObjObject::Draw();
}

void Enemy::Finalize()
{
	BaseObjObject::Finalize();
}

void Enemy::OnCollision(const CollisionInfo &info)
{
	//CollisionManager::RemoveCollider(collider);
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(DeadPos);
		IsDead = true;
	}
}
