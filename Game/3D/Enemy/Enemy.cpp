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

	//座標
	SetPosition(DeadPos);

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,-0.2f,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	//パーティクル
	PopParticle = new ParticleObject();
	DeadParticle = new ParticleObject();
	PopParticle->Initialize();
	DeadParticle->Initialize();
}

void Enemy::Update(Camera *camera)
{
	this->camera = camera;

	//出現予定
	if(!IsPop){
		//拍終了
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

	//死亡時
	if(IsDead){
		if(IsDeadAudioOnce) IsDeadAudioOnce = false;
		DeadParticleApp();
		DeadParticle->Update(this->camera);
	}

	//生存時
	if(!IsDead && IsPop){

		//拍終了
		if(IsBeatEnd){

			//待機、移動、切り替え
			if(!IsWait) IsWait = true;
			else if(IsWait) IsWait = false;

			//移動
			if(IsWait)world.translation += direction*2.5f;

			//スケール
			IsScale  = true;
			//拍終了
			IsBeatEnd = false;
		}

		//スケール遷移
		if(IsScale){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScale = false;
			}
		}


		//落下処理
		if(!IsGround){
			DeadParticlePos = this->GetPosition();
			IsDeadParticleOnce = true;
			//地面がない場所では死亡
			IsDead = true;
		}

		//行列、カメラ更新
		BaseObjObject::Update(this->camera);
		//コライダー更新
		collider->Update();

		//球コライダー取得
		SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
		assert(sphereCollider);


		//球の上端から球の下端までの例キャスト用レイを準備
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = {0,-1,0,0};
		RaycastHit raycastHit;

		//接地状態
		if(IsGround){
			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			//接地を維持
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
				IsGround = true;
				world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
				//行列の更新など
				BaseObjObject::Update(this->camera);
			}
			//地面がないので落下
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

		//自身の座標を軸に[-1, 1]ランダム
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

	//回転
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);
}
