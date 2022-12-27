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

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//回転
	if(direction.x == +1) world.rotation.y = XMConvertToRadians(90.f);
	else if(direction.x == -1) world.rotation.y = XMConvertToRadians(-90.f);
	else if(direction.z == +1) world.rotation.y = XMConvertToRadians(0.f);
	else if(direction.z == -1) world.rotation.y = XMConvertToRadians(180.f);

	//コライダー追加
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void Enemy::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsDead){

		//拍終了
		if(IsBeatEnd){

			//移動
			world.translation += direction*2.5f;

			//スケール
			IsScale  = true;

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
			//下向き加速度
			const float fallAcc = -0.01f;
			const float fallVYMin = -0.5f;
			//加速
			fallV.y = max(fallV.y + fallAcc, fallVYMin);
			//移動
			world.translation.x += fallV.x;
			world.translation.y += fallV.y;
			world.translation.z += fallV.z;
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
		//落下状態
		else if(fallV.y <= 0.0f){
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius()*2.0f)){
				//着地
				IsGround = true;
				world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
				//行列の更新など
				BaseObjObject::Update(this->camera);
			}
		}
	}

	BaseObjObject::Update(this->camera);
}

void Enemy::Draw()
{
	if(IsDead) return;

	BaseObjObject::Draw();
}

void Enemy::Finalize()
{
	BaseObjObject::Finalize();
}

void Enemy::OnCollision(const CollisionInfo &info)
{
	//CollisionManager::RemoveCollider(collider);
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(DeadPos);
		IsDead = true;
	}
}
