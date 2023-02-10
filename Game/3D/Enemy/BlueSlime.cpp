#include "BlueSlime.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

BlueSlime::~BlueSlime()
{
}

void BlueSlime::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMin = {0.7f,0.7f,0.7f};

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();
}

void BlueSlime::Update(Camera *camera, Vector3 playerPos)
{
	this->camera = camera;
	if(!IsAlive) return;

	//距離計測
	Vector3 pos = playerPos - world.translation;
	distance = pos.length();

	if(-20 <= distance && distance <= 20)		{
		IsInvisible = false;
	}
	else if(-20 > distance || distance > 20)	IsInvisible = true;

	//死亡
	if(IsDead){
		DeadParticleApp();
	}
	DeadParticle->Update(this->camera);
	
	if(IsInvisible) return;
	//生存
	if(!IsDead){
		//拍終了
		if(IsBeatEnd){
			//スケール
			IsScaleEasing  = true;
			//拍終了
			IsBeatEnd = false;
		}
		//スケール遷移
		if(IsScaleEasing){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScaleEasing = false;
			}
		}
	}
	BaseObjObject::Update(this->camera);
}

void BlueSlime::Draw()
{
	if(!IsAlive) return;
	if(IsInvisible) return;

	BaseObjObject::Draw();
}

void BlueSlime::ParticleDraw()
{
	if(!IsAlive) return;
	
	if(IsDead){
		DeadParticle->Draw();
	}
}

void BlueSlime::Finalize()
{
	DeadParticle->Finalize();
	delete DeadParticle;

	BaseObjObject::Finalize();
}

void BlueSlime::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;
	if(IsInvisible) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsDead = true;
		IsDeadOnceAudio = true;
		IsDeadOnceParticle = true;

		SetPosition(IsNotAlivePos);
		world.UpdateMatrix();
		collider->Update();

		DeadParticlePos = info.objObject->GetPosition();
	}
}

void BlueSlime::Pop(Vector3 pos)
{
	SetPosition(pos);
	IsAlive =true;
}

void BlueSlime::DeadParticleApp()
{
	if(!IsDeadOnceParticle) return;
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

	IsDeadOnceParticle = false;
}
