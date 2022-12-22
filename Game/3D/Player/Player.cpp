#include "Player.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;


Player::~Player()
{
	Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//コライダーの追加
	float radius = 0.6f;
	//半径文だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));

	//当たり判定属性
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;

	//入力
	Input* input = Input::GetInstance();
	if(input->Trigger(DIK_UP)){
		world.translation.z += 2.f;
		world.rotation.y = 0;
	}
	else if(input->Trigger(DIK_DOWN)){
		world.translation.z -= 2.f;
		world.rotation.y = XMConvertToRadians(180);
	}
	else if(input->Trigger(DIK_RIGHT)){
		world.translation.x += 2.f;
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(input->Trigger(DIK_LEFT)){
		world.translation.x -= 2.f;
		world.rotation.y = XMConvertToRadians(-90);
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
	//ジャンプ操作
	else if(input->Trigger(DIK_SPACE)){
		IsGround = false;
		const float jumpVYFist = 0.2f;	//ジャンプ時上向き初速
		fallV = {0,jumpVYFist, 0};
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

	BaseObjObject::Update(this->camera);
}

void Player::Draw3D()
{
	BaseObjObject::Draw();
}


void Player::Finalize()
{
	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{

}

