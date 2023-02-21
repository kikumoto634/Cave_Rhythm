#include "Skelton.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

Skelton::~Skelton()
{
}

void Skelton::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMin = {0.7f,0.7f,0.7f};

	SetPosition(NotAlivePos);
	SetRotation({0,DirectX::XMConvertToRadians(180),0.f});
	world.UpdateMatrix();

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	//球コライダー取得
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
	collider->Update();

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();
	PopParticle = new ParticleObject();
	PopParticle->Initialize();
}

void Skelton::Update(Camera *camera, Vector3 playerPos)
{
	this->camera = camera;
	if(!IsNotApp) return;

	//距離計測
	Vector3 pos = playerPos - world.translation;
	distance = pos.length();
	if(-20 <= distance && distance <= 20)		{
		IsInvisible = false;
	}
	else if(-20 > distance || distance > 20)	IsInvisible = true;

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

			//待機カウント
			if(waitCount >= WaitCount){
				//移動
				IsMoveEasing = true;
				targetPos = playerPos;
				Movement();
				currentPos = GetPosition();
				OldPosition = currentPos;
				waitCount = 0;
			}
			else{
				waitCount++;
			}
		}

		//移動
		if(IsMoveEasing){
			world.translation = Easing_Linear_Point2(currentPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));
		
			if(moveEasingFrame >= 1.f){
				IsMoveEasing = false;
				world.translation = movePosition;
				currentPos = {};
				movePosition = {};
				moveEasingFrame = 0;
			}
		}

		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = {RayDir.x, RayDir.y, RayDir.z, 0};
		RaycastHit raycastHit;
		if(IsMoveEasing){
			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.1f;
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
				movePosition = OldPosition;
			}
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

void Skelton::Draw()
{
	if(IsDead) return;
	if(IsPop) return;
	if(IsInvisible) return;

	BaseObjObject::Draw();
}

void Skelton::ParticleDraw()
{
	if(IsDead){
		DeadParticle->Draw();
	}
	else if(IsPop){
		PopParticle->Draw();
	}
}

void Skelton::Finalize()
{
	DeadParticle->Finalize();
	delete DeadParticle;
	PopParticle->Finalize();
	delete PopParticle;

	BaseObjObject::Finalize();
}

void Skelton::OnCollision(const CollisionInfo &info)
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

void Skelton::Pop(Vector3 pos)
{
	PopParticlePos = pos;
	IsNotApp = true;
	IsPop = true;
}

void Skelton::Reset()
{
	IsNotApp = false;
	IsDead = false;

	waitCount = 0;

	IsMoveEasing = false;
	currentPos = {};
	movePosition = {};
	OldPosition = {};
	moveEasingFrame = 0;
}

void Skelton::Movement()
{
	if(IsInvisible) return;

	Vector2 baseVector = {1,0};
	Vector2 subVector = {GetPosition().x - targetPos.x, GetPosition().z - targetPos.z};

	float lengthBase = baseVector.length();
	float lengthSub = subVector.length();

	float cos = baseVector.dot(subVector)/(lengthBase*lengthSub);

	float sita = acosf(cos);
	sita = sita*(180/3.14159265f);
	
	//右
	if(sita >= 135){
		movePosition = world.translation + Vector3{2.f,0,0};
		SetRotation({0,XMConvertToRadians(90),0});
		RayDir = {1,0,0};
	}
	//左
	else if(45 >= sita){
		movePosition = world.translation + Vector3{-2.f,0,0};
		SetRotation({0,XMConvertToRadians(-90),0});
		RayDir = {-1,0,0};
	}
	//下
	else if(sita > 45 && 135 > sita){

		//下
		if(subVector.y > 0){
			movePosition = world.translation + Vector3{0,0,-2.f};
			SetRotation({0,XMConvertToRadians(180),0});
			RayDir = {0,0,-1};
		}
		//上
		else if(subVector.y < 0){
			movePosition = world.translation + Vector3{0,0,2.f};
			SetRotation({0,0,0});
			RayDir = {0,0,1};
		}
	}
}

void Skelton::PopParticleApp()
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

void Skelton::DeadParticleApp()
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
