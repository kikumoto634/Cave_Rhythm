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

	//�p�[�e�B�N��
	PopParticle = new ParticleObject();
	PopParticle->Initialize();

	//��]
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);
}

void Enemy::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsPop){
		//���I��
		if(IsBeatEnd){
			popCount += 1;
			IsBeatEnd = false;

			if(popCount >= POP_COUNT){
				IsPop = true;

				//�R���C�_�[�ǉ�
				float radius = 0.6f;
				SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));
				collider->SetAttribute(COLLISION_ATTR_ENEMYS);
			}
		}

		PopParticleApp();

		PopParticle->Update(this->camera);
	}

	if(!IsDead && IsPop){

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
			//�n�ʂ��Ȃ��ꏊ�ł͎��S
			IsDead = true;
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
	}

	BaseObjObject::Update(this->camera);
}

void Enemy::Draw()
{
	if(!IsPop) return;
	if(IsDead) return;

	BaseObjObject::Draw();
}

void Enemy::ParticleDraw()
{
	if(IsPop) return;

	PopParticle->Draw();
}

void Enemy::Finalize()
{
	PopParticle->Finalize();
	BaseObjObject::Finalize();
}

void Enemy::OnCollision(const CollisionInfo &info)
{
	if(!IsPop) return;
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(DeadPos);
		IsDead = true;
	}
}

void Enemy::PopParticleApp()
{
	
	for (int i = 0; i < 10; i++) {

		const float rnd_vel = 0.025f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.05f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		PopParticle->ParticleSet(40, GetPosition(),vel,acc,0.1f,0.5f,1,{1,0,1,1});
		PopParticle->ParticleAppearance();
	}
}
