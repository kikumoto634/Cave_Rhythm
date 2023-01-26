#include "Enemy.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

#ifdef _DEBUG
	#include "../../../Engine/input/Input.h"
#endif // _DEBUG


Enemy::~Enemy()
{
	//Finalize();
}

void Enemy::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//座標
	SetPosition(DeadPos);
	world.UpdateMatrix();

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	IsDead = true;
	IsNotApp = true;

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
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

	if(IsNotApp) return;

	//出現予定
	if(!IsPop){
		//拍終了
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

	//死亡時
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

	//生存時
	if(!IsDead && IsPop){

		//拍終了
		if(IsBeatEnd){

			//待機、移動、切り替え
			waitNum++;

			//移動
			if(waitNum >= WaitTime){
				waitNum = 0;
				//world.translation += direction*2.5f;
				IsMoveEasing = true;
				movePosition = world.translation + direction*2.5f;
				moveEasingPos = world.translation;
			}

			//スケール
			IsScale  = true;
			//拍終了
			IsBeatEnd = false;
		}

		//移動イージング
		if(IsMoveEasing){
			world.translation = Easing_Linear_Point2(moveEasingPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));
		
			if(world.translation.x == movePosition.x && world.translation.z == movePosition.z){
				IsMoveEasing = false;
				world.translation = movePosition;
				moveEasingPos = {};
				movePosition = {};
				moveEasingFrame = 0;
			}
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
	if(IsNotApp) return;
	if(!IsPop) return;
	if(IsDead) return;

	BaseObjObject::Draw();
}

void Enemy::ParticleDraw()
{
	if(IsNotApp) return;

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

void Enemy::BeginAppearance()
{
	IsDead = false;
	IsPop = false;
	IsNotApp = false;
}

void Enemy::Reset()
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

void Enemy::PopParticleApp()
{
	
	for (int i = 0; i < 10; i++) {

		//自身の座標を軸に[-1, 1]ランダム
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

void Enemy::DeadParticleApp()
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

void Enemy::SetDirection(Vector3 _dir)
{
	direction = _dir;

	//回転
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);
}
