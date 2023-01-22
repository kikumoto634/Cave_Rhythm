#include "TrainingDummy.h"
#include "../../Collision/SphereCollider.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;

TrainingDummy::~TrainingDummy()
{
	//Finalize();
}

void TrainingDummy::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//�T�C�Y�ύX�̍ŏ��l�ύX
	ScaleMin = {0.7f, 0.7f, 0.7f};

	SetPosition({0.f,-3.5f, 0.f});

	//�R���C�_�[
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void TrainingDummy::Update(Camera *camera)
{
	this->camera = camera;
	
	if(IsDead){
		if(IsDeadAudioOnce){
			IsDeadAudioOnce = false;
			appearanceResetFrame = 0;
		}

		if(appearanceResetFrame >= AppearanceResetFrame){
			IsDead = false;
			SetPosition(RespawnPos);
			return;
		}
		appearanceResetFrame++;
	}
	else if(!IsDead){
		//���I��
		if(IsBeatEnd){
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
			}
		}
	}
	BaseObjObject::Update(this->camera);
}

void TrainingDummy::Draw()
{
	if(IsDead) return;

	BaseObjObject::Draw();
}

void TrainingDummy::Finalize()
{
	BaseObjObject::Finalize();
}

void TrainingDummy::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsDead = true;
		IsDeadAudioOnce = true;
		RespawnPos = GetPosition();
		SetPosition(DeadPos);
	}
}
