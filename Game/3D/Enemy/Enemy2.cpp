#include "Enemy2.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

#ifdef _DEBUG
	#include "../../../Engine/input/Input.h"
#endif // _DEBUG


Enemy2::~Enemy2()
{
	//Finalize();
}

void Enemy2::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//���W
	SetPosition(DeadPos);
	SetRotation({0,XMConvertToRadians(180),0});
	world.UpdateMatrix();

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	IsDead = true;
	IsNotApp = true;

	//�R���C�_�[
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	//�p�[�e�B�N��
	PopParticle = new ParticleObject();
	DeadParticle = new ParticleObject();
	PopParticle->Initialize();
	DeadParticle->Initialize();
}

void Enemy2::Update(Camera *camera, Vector3 targetPos)
{
	this->camera = camera;

	if(IsNotApp) return;

	//�o���\��
	if(!IsPop){
		//���I��
		if(IsBeatEnd){
			popCount += 1;
			IsBeatEnd = false;

			if(popCount >= POP_COUNT){
				SetPosition(popPosition);
				popCount = 0;
				IsPop = true;
				IsDead = false;
				return;
			}
		}

		PopParticleApp();
	}
	PopParticle->Update(this->camera);

	//���S��
	if(IsDead){
		if(IsDeadAudioOnce) {
			IsDeadAudioOnce = false;
			appearanceResetFrame = 0;
		}
		DeadParticleApp();

		if(appearanceResetFrame >= AppearanceResetFrame){
			Reset();
			return;
		}
		appearanceResetFrame++;
	}
	DeadParticle->Update(this->camera);

	//������
	if(!IsDead && IsPop){

		//���I��
		if(IsBeatEnd){

			//�ҋ@�A�ړ��A�؂�ւ�
			waitNum++;

			//�ړ�
			if(waitNum >= WaitTime){
				waitNum = 0;
				IsMoveEasing = true;
				//�ړ���w��
				TargetMove(targetPos);
				//���ݒn�_
				moveEasingPos = world.translation;
			}

			//�X�P�[��
			IsScale  = true;
			//���I��
			IsBeatEnd = false;
		}

		//�ړ��C�[�W���O
		if(IsMoveEasing){
			world.translation = Easing_Linear_Point2(moveEasingPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));
		
			if(moveEasingFrame >= 1.f){
				IsMoveEasing = false;
				world.translation = movePosition;
				moveEasingPos = {};
				movePosition = {};
				moveEasingFrame = 0;
			}
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

void Enemy2::Draw()
{
	if(IsNotApp) return;
	if(!IsPop) return;
	if(IsDead) return;

	BaseObjObject::Draw();
}

void Enemy2::ParticleDraw()
{
	if(IsNotApp) return;

	if(!IsPop){
		PopParticle->Draw();
	}

	if(IsDead){
		DeadParticle->Draw();
	}
}

void Enemy2::Finalize()
{
	PopParticle->Finalize();
	DeadParticle->Finalize();
	BaseObjObject::Finalize();
}

void Enemy2::OnCollision(const CollisionInfo &info)
{
	if(IsNotApp) return;
	if(!IsPop) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		DeadParticlePos = info.objObject->GetPosition();
		SetPosition(DeadPos);
		IsDead = true;
		IsDeadAudioOnce = true;
		IsDeadParticleOnce = true;
	}
	else if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(DeadPos);
		IsDead = true;
	}
}

void Enemy2::BeginAppearance()
{
	IsDead = false;
	IsPop = false;
	IsNotApp = false;
}

void Enemy2::Reset()
{
	IsGround = true;
	fallV = {};

	IsNotApp = true;
	appearanceResetFrame = 0;

	IsDead = false;
	IsDeadAudioOnce = false;

	DeadParticlePos = {};
	IsDeadParticleOnce = false;

	IsScale = false;

	waitNum = 0;

	direction = {};

	IsPop = false;
	popCount = 0;
	popPosition = {};

	world.scale = {1.0f, 1.0f, 1.0f};
	world.translation = DeadPos;
}

void Enemy2::PopParticleApp()
{
	
	for (int i = 0; i < 10; i++) {

		//���g�̍��W������[-1, 1]�����_��
		const Vector3 rnd_pos = popPosition;
		const float range = 1.5f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * range - range/2.0f;
		pos.y = -0.5f;
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

void Enemy2::DeadParticleApp()
{
	if(!IsDeadParticleOnce) return;

	for (int i = 0; i < 10; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DeadParticle->ParticleSet(AppearanceResetFrame,DeadParticlePos,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		DeadParticle->ParticleAppearance();
	}

	IsDeadParticleOnce = false;
}

void Enemy2::TargetMove(Vector3 targetPos)
{
	Vector2 baseVector = {1,0};
	Vector2 subVector = {GetPosition().x - targetPos.x, GetPosition().z - targetPos.z};

	float lengthBase = baseVector.length();
	float lengthSub = subVector.length();

	float cos = baseVector.dot(subVector)/(lengthBase*lengthSub);

	float sita = acosf(cos);
	sita = sita*(180/3.14159265f);
	
	//�E
	if(sita >= 135){
		movePosition = world.translation + Vector3{2.f,0,0};
		SetRotation({0,XMConvertToRadians(90),0});
	}
	//��
	else if(45 >= sita){
		movePosition = world.translation + Vector3{-2.f,0,0};
		SetRotation({0,XMConvertToRadians(-90),0});
	}
	//��
	else if(sita > 45 && 135 > sita){

		//��
		if(subVector.y > 0){
			movePosition = world.translation + Vector3{0,0,-2.f};
			SetRotation({0,XMConvertToRadians(180),0});
		}
		//��
		else if(subVector.y < 0){
			movePosition = world.translation + Vector3{0,0,2.f};
			SetRotation({0,0,0});
		}
	}
}
