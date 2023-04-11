#include "Boss1.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "Easing.h"

using namespace std;

void Boss1::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMax = {1.3f,1.3f,1.3f};
	ScaleMin = {1.f,1.f,1.f};

	SetPosition(NotAlivePos);
	SetRotation({0,DirectX::XMConvertToRadians(180),0.f});
	world.UpdateMatrix();

	hp = FullHP;

	//パーティクル
	DeadParticle = make_unique<ParticleObject>();
	DeadParticle->Initialize();
	SummonParticle = make_unique<ParticleObject>();
	SummonParticle->Initialize();

	//コライダー
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->Update();
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
	}
	else if(-13 > distance || distance > 13)	{
		IsInvisible = true;
	}

	//ダメージ
	if(IsDamage){
		damageResetCurFrame++;

		if(hp <= 0){
			IsDead = true;
			IsDeadOnceAudio = true;
			IsDeadOnceParticle = true;
			DeadParticlePos = GetPosition();
			SetPosition(NotAlivePos);
			world.UpdateMatrix();
			collider->Update();
			return;
		}

		if(IsDeadOnceAudio){
			IsDeadOnceAudio = false;
		}

		//無敵時間内
		Vector4 color;
		if(damageResetCurFrame / 6 == 0){
			color = {0.0f, 0.0f, 0.0f, 1.0f};
		}
		else{
			color = {1.0f, 0.0f, 0.0f, 1.0f};
		}
		object->SetColor(color);

		//無敵時間
		if(damageResetCurFrame >= DamageResetFrame){
			damageResetCurFrame = 0;
			color = {1.0f,1.0f,1.0f,1.0f};
			object->SetColor(color);
			IsDamage = false;
		}
	}

	if(IsInvisible) return;
	//生存
	if(!IsDead){
		//拍終了
		if(IsBeatEnd){
			//スケール
			IsScaleEasing  = true;
			//拍終了
			IsBeatEnd = false;
			//カウント
			patternCount++;

			if(patternCount == 2){
				IsSummon = true;
				IsMove = false;
			}
			else if(patternCount == 25){
				IsSummon = false;
				IsMove = true;
			}

			//追撃
			if(IsMove){
				moveWaitCurCount++;
					if(moveWaitCurCount >= MoveWaitCount){
					//移動
					IsMoveEasing = true;
					OldPosition = GetPosition();
					if(!IsComeBack)	targetPos = playerPos;
					else if(IsComeBack) targetPos = homePos;
					Movement();
					currentPos = GetPosition();
					moveWaitCurCount = 0;
				}
			}
			//召喚
			if(IsSummon){
				Summon();
			}
		}
		//スケール遷移
		if(IsScaleEasing){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScaleEasing = false;
			}
		}

		//移動
		if(IsMoveEasing){
			world.translation = Easing_Linear_Point2(currentPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));
		
			Vector2 subVector = {GetPosition().x - targetPos.x, GetPosition().z - targetPos.z};
			if(subVector.length() <= 1.f){
			
				if(IsMove){
					IsMove = false;
					patternCount = 0;
				}

				//戻り終わった
				if(IsComeBack){
					IsComeBack = false;
					SetRotation({0,XMConvertToRadians(180),0});
				}
				//戻る
				else if(!IsComeBack){
					IsComeBack = true;
				}

				movePosition = OldPosition;
			}

			if(moveEasingFrame >= 1.f){
				IsMoveEasing = false;
				world.translation = movePosition;
				currentPos = {};
				movePosition = {};
				moveEasingFrame = 0;
			}
		}
	}
	BaseObjObject::Update(this->camera);
}

void Boss1::ParticleUpdate()
{
	//死亡
	if(IsDead){
		if(IsDeadOnceAudio){
			IsDeadOnceAudio = false;
			appDeadParFrame = 0;
		}

		if(appDeadParFrame >= AppDeadParMaxFrame){
			appDeadParFrame = AppDeadParMaxFrame;
			return;
		}

		appDeadParFrame++;

		DeadParticleApp();
		DeadParticle->Update(this->camera);
	}

	//召喚
	if(IsSummon){
		SummonParticlePos = GetPosition();

		if(appSummonParFrame >= AppSummonParMaxFrame){
			appSummonParFrame = 0;
		}

		appSummonParFrame++;

		SummonParticleApp();
		SummonParticle->Update(this->camera);
	}
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
	if(IsSummon){
		SummonParticle->Draw();
	}
}

void Boss1::Finalize()
{
	DeadParticle->Finalize();
	SummonParticle->Finalize();

	BaseObjObject::Finalize();
}

void Boss1::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;
	if(IsInvisible) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		
		if(!IsDamage){
			hp = hp - 1;
			IsDeadOnceAudio = true;
			IsDamage = true;
		}

		IsComeBack = true;
	}
}

void Boss1::Pop(Vector3 pos)
{
	world.translation = pos;
	world.UpdateMatrix();
	homePos = pos;
	IsNotApp = true;
}

void Boss1::Movement()
{
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
	}
	//左
	else if(45 >= sita){
		movePosition = world.translation + Vector3{-2.f,0,0};
		SetRotation({0,XMConvertToRadians(-90),0});
	}
	//下
	else if(sita > 45 && 135 > sita){

		//下
		if(subVector.y > 0){
			movePosition = world.translation + Vector3{0,0,-2.f};
			SetRotation({0,XMConvertToRadians(180),0});
		}
		//上
		else if(subVector.y < 0){
			movePosition = world.translation + Vector3{0,0,2.f};
			SetRotation({0,0,0});
		}
	}
}

void Boss1::Summon()
{
	if(!IsSummon) return;
	if(summonEnemyPosNum >= SummonEnemyPosNumMax) {
		IsSummon = false;
		summonEnemyPosNum = 0;
		return;
	}

	summonEnemyPosNum++;
	IsSummonEnemyPop = true;
}

void Boss1::DeadParticleApp()
{
	if(!IsDeadOnceParticle) return;

	Vector3 vel{};
	const float rnd_vel = 0.08f;
	float randomVel = rnd_vel - rnd_vel / 2.0f;
	vel.y = 0.06f;

	Vector3 acc{};
	acc.y = -0.005f;

	for (int i = 0; i < 10; i++) {
		vel.x = (float)rand() / RAND_MAX * randomVel;
		vel.z = (float)rand() / RAND_MAX * randomVel;

		DeadParticle->ParticleSet(AppDeadParMaxFrame,DeadParticlePos,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		DeadParticle->ParticleAppearance();
	}

	IsDeadOnceParticle = false;
}

void Boss1::SummonParticleApp()
{
	if(!IsSummon) return;
	const float rnd_pos = 3.f;
	Vector3 pos;
	pos.x = (float)rand()/RAND_MAX * rnd_pos - rnd_pos/2.0f;
	pos.y = -1.f;
	pos.z = (float)rand()/RAND_MAX * rnd_pos - rnd_pos/2.0f;
	pos += SummonParticlePos;

	const float rnd_vel = 0.08f;
	Vector3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = 0.06f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	Vector3 acc{};
	acc.y = 0.001f;

	SummonParticle->ParticleSet(AppSummonParMaxFrame,pos,vel,acc,0.4f,0.0f,1,{0.0f,0.4f,0.0f,0.4f});
	SummonParticle->ParticleAppearance();
}
