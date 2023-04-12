#include "BlueSlime.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "Easing.h"

BlueSlime::~BlueSlime()
{
}

void BlueSlime::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMin = {0.7f,0.7f,0.7f};

	SetPosition(NotAlivePos);
	SetRotation({0,DirectX::XMConvertToRadians(180),0.f});
	world.UpdateMatrix();

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();
	PopParticle = new ParticleObject();
	PopParticle->Initialize();
}

void BlueSlime::Update(Camera *camera, Vector3 playerPos)
{
	this->camera = camera;
	if(!IsNotApp) return;

	//距離計測
	Vector3 pos = playerPos - world.translation;
	distance = pos.length();
	//距離に応じたライト表現
	if(IsCaveLight){
		if(-DrawingRange_Half <= distance && distance <= DrawingRange_Half){
			object->OnLighting();
		}
		else if(-DrawingRange_Half > distance || distance > DrawingRange_Half){
			object->OffLighting();
		}
	}
	else if(!IsCaveLight){
		object->OnLighting();
	}

	//距離に応じたコライダーの削除
	if(-DrawingRange_Not <= distance && distance <= DrawingRange_Not)		{
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
	else if(-DrawingRange_Not > distance || distance > DrawingRange_Not)	{
		IsInvisible = true;

		if(IsCollision){
			if(collider){
				//コリジョンマネージャーから登録を解除する
				CollisionManager::GetInstance()->RemoveCollider(collider);
			}
			IsCollision = false;
		}
	}

	//出現
	if(IsPop){
		if(IsBeatEnd){
			popCount++;
			IsBeatEnd = false;

			if(popCount >= POP_COUNT){
				SetPosition(PopParticlePos);
				PopParticlePos = {0,50,0};
				popCount = 0;
				IsPop = false;
				IsDead = false;
				return;
			}
		}
	}
	PopParticleApp();
	PopParticle->Update(this->camera);

	//死亡
	if(IsDead && !IsPop){
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
	if(!IsDead && !IsPop){
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
	if(IsDead) return;
	if(IsPop) return;
	if(IsInvisible) return;

	BaseObjObject::Draw();
}

void BlueSlime::ParticleDraw()
{
	if(IsDead){
		DeadParticle->Draw();
	}
	else if(IsPop){
		PopParticle->Draw();
	}
}

void BlueSlime::Finalize()
{
	DeadParticle->Finalize();
	delete DeadParticle;
	PopParticle->Finalize();
	delete PopParticle;

	BaseObjObject::Finalize();
}

void BlueSlime::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;
	if(IsPop) return;
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

void BlueSlime::Pop(Vector3 pos)
{
	PopParticlePos = pos;
	IsNotApp = true;
	IsPop = true;
}

void BlueSlime::Reset()
{
	IsNotApp = false;
	IsDead = false;
}

void BlueSlime::PopParticleApp()
{
	for (int i = 0; i < 10; i++) {

		//自身の座標を軸に[-1, 1]ランダム
		const Vector3 rnd_pos = PopParticlePos;
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
