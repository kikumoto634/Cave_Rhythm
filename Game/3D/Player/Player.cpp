#include "Player.h"
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

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//入力
	input = Input::GetInstance();

	//コライダーの追加
	float radius = 0.6f;
	//半径文だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));

	//当たり判定属性
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//球コライダー取得
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//武器
	weapon = new PlayerWeapon();
	weapon->Initialize("sphere", true);
	weapon->GetmatWorld().parent = &world;
}

void Player::Update(Camera *camera)
{
	this->camera = camera;
	
	//移動
	Movement();

	if(input->Trigger(DIK_RETURN)){
		weapon->Attack();
	}

	//拍終了
	if(IsBeatEnd){
		//サイズ変更
		if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
			IsBeatEnd = false;
		}
	}

	//ダメージ
	DamageUpdate();

	//重力
	GravityFall();

	//行列、カメラ更新
	BaseObjObject::Update(this->camera);
	//コライダー更新
	collider->Update();

	//地面接触判定
	GroundCollider();

	//武器
	weapon->Update(this->camera);

	//ベース更新
	BaseObjObject::Update(this->camera);
}

void Player::Draw()
{
	weapon->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	weapon->Finalize();
	delete weapon;
	weapon = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	//敵接触
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		Damage();
	}
}


void Player::Movement()
{
	//歩行
	if(input->Trigger(DIK_UP)){
		world.translation.z += 2.5f;
		world.rotation.y = 0;
	}
	else if(input->Trigger(DIK_DOWN)){
		world.translation.z -= 2.5f;
		world.rotation.y = XMConvertToRadians(180);
	}
	else if(input->Trigger(DIK_RIGHT)){
		world.translation.x += 2.5f;
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(input->Trigger(DIK_LEFT)){
		world.translation.x -= 2.5f;
		world.rotation.y = XMConvertToRadians(-90);
	}
}

void Player::Damage()
{
	if(IsDamage) return;
	IsDamage = true;
	HP -= 1;
}

void Player::DamageUpdate()
{
	if(!IsDamage) return ;

	damageCurrentFrame += 1;

	//無敵時間内
	Vector4 color = (damageCurrentFrame % 6 == 1) ? color = {0.0f, 0.0f, 0.0f, 0.0f} : color = {1.0f, 0.0f, 0.0f, 1.0f};
	object->SetColor(color);

	//無敵時間終了
	if(damageCurrentFrame < DamageFrame) return;
	damageCurrentFrame = 0;
	object->SetColor({1.0f,1.0f,1.0f, 1.0f});
	IsDamage = false;
}

void Player::GravityFall()
{
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
}

void Player::GroundCollider()
{
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

