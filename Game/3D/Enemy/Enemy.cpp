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

	//���W
	SetPosition(DeadPos);

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//�R���C�_�[
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,-0.2f,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	//�p�[�e�B�N��
	PopParticle = new ParticleObject();
	DeadParticle = new ParticleObject();
	PopParticle->Initialize();
	DeadParticle->Initialize();
}

void Enemy::Update(Camera *camera)
{
	this->camera = camera;

	//�o���\��
	if(!IsPop){
		//���I��
		if(IsBeatEnd){
			popCount += 1;
			IsBeatEnd = false;

			if(popCount >= POP_COUNT){
				SetPosition(popPosition);
				IsPop = true;
			}
		}

		PopParticleApp();

		PopParticle->Update(this->camera);
	}

	//���S��
	if(IsDead){
		if(IsDeadAudioOnce) IsDeadAudioOnce = false;
		DeadParticleApp();
		DeadParticle->Update(this->camera);
	}

	//������
	if(!IsDead && IsPop){

		//���I��
		if(IsBeatEnd){

			//�ҋ@�A�ړ��A�؂�ւ�
			if(!IsWait) IsWait = true;
			else if(IsWait) IsWait = false;

			//�ړ�
			if(IsWait)world.translation += direction*2.5f;

			//�X�P�[��
			IsScale  = true;
			//���I��
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
			DeadParticlePos = this->GetPosition();
			IsDeadParticleOnce = true;
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
	if(!IsPop){
		PopParticle->Draw();
	}

	if(IsDead){
		DeadParticle->Draw();
	}
}

void Enemy::Finalize()
{
	PopParticle->Finalize();
	DeadParticle->Finalize();
	BaseObjObject::Finalize();
}

void Enemy::OnCollision(const CollisionInfo &info)
{
	if(!IsPop) return;
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		DeadParticlePos = info.objObject->GetPosition();
		SetPosition(DeadPos);
		IsDead = true;
		IsDeadAudioOnce = true;
		IsDeadParticleOnce = true;
	}
}

void Enemy::PopParticleApp()
{
	
	for (int i = 0; i < 10; i++) {

		//���g�̍��W������[-1, 1]�����_��
		const Vector3 rnd_pos = popPosition;
		const float range = 1.5f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * range - range/2.0f;
		pos.y = 0.0f;
		pos.z = (float)rand() / RAND_MAX * range - range/2.0f;
		pos += rnd_pos;

		const float rnd_vel = 0.025f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.05f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		PopParticle->ParticleSet(40,pos,vel,acc,0.4f,0.0f,1,{0.6f,0.3f,0.2f,0.4f});
		PopParticle->ParticleAppearance();
	}
}

void Enemy::DeadParticleApp()
{
	if(!IsDeadParticleOnce) return;

	for (int i = 0; i < 10; i++) {
		const float rnd_vel = 0.05f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DeadParticle->ParticleSet(50,DeadParticlePos,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		DeadParticle->ParticleAppearance();
	}

	IsDeadParticleOnce = false;
}

void Enemy::SetDirection(Vector3 _dir)
{
	direction = _dir;

	//��]
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);
}
