#include "Boss1.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

Boss1::~Boss1()
{
}

void Boss1::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMax = {1.5f,1.5f,1.5f};
	ScaleMin = {1.f,1.f,1.f};

	SetPosition(NotAlivePos);
	SetRotation({0,DirectX::XMConvertToRadians(180),0.f});
	world.UpdateMatrix();

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();
}

void Boss1::Update(Camera *camera, Vector3 playerPos)
{
	this->camera = camera;
	if(!IsNotApp) return;

	//距離計測
	Vector3 pos = playerPos - world.translation;
	distance = pos.length();
	if(-13 <= distance && distance <= 13)		{
		IsInvisible = false;

		if(!IsCollision){
			//コライダー
			float radius = 0.6f;
			SetCollider(new SphereCollider(XMVECTOR{0,0.0,0}, radius));
			collider->SetAttribute(COLLISION_ATTR_ENEMYS);
			collider->Update();
			IsCollision = true;
		}
	}
	else if(-13 > distance || distance > 13)	{
		IsInvisible = true;

		if(IsCollision){
			if(collider){
				//コリジョンマネージャーから登録を解除する
				CollisionManager::GetInstance()->RemoveCollider(collider);
			}
			IsCollision = false;
		}
	}

	//死亡
	if(IsDead){
		if(IsDeadOnceAudio){
			IsDeadOnceAudio = false;
			appearancePopFrame = 0;
		}
		DeadParticleApp();

		if(appearancePopFrame >= AppearanceResetFrame){
			Reset();
			return;
		}
		appearancePopFrame++;
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

void Boss1::Draw()
{
	if(IsDead) return;
	if(IsInvisible) return;

	BaseObjObject::Draw();
}

void Boss1::ParticleDraw()
{
	if(IsDead){
		DeadParticle->Draw();
	}
}

void Boss1::Finalize()
{
	DeadParticle->Finalize();
	delete DeadParticle;

	BaseObjObject::Finalize();
}

void Boss1::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;
	if(IsInvisible) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsDead = true;
		IsDeadOnceAudio = true;
		IsDeadOnceParticle = true;

		SetPosition(NotAlivePos);
		world.UpdateMatrix();
		collider->Update();

		DeadParticlePos = info.objObject->GetPosition();
	}
	else if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(NotAlivePos);
		world.UpdateMatrix();
		collider->Update();
		IsDead = true;
	}
}

void Boss1::Pop(Vector3 pos)
{
	world.translation = pos;
	world.UpdateMatrix();
	IsNotApp = true;
}

void Boss1::Reset()
{
	IsNotApp = false;
	IsDead = false;
}

void Boss1::DeadParticleApp()
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
