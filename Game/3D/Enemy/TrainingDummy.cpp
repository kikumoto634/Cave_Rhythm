#include "TrainingDummy.h"
#include "../../Collision/SphereCollider.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;

TrainingDummy::~TrainingDummy()
{
	//Finalize();
}

void TrainingDummy::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	SetPosition({0.f,-3.5f, 0.f});

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void TrainingDummy::Update(Camera *camera)
{
	this->camera = camera;
	
	if(IsDead){
		if(IsDeadAudioOnce){
			IsDeadAudioOnce = false;
			appearanceResetFrame = 0;
		}

		if(appearanceResetFrame >= AppearanceResetFrame){
			IsDead = false;
			SetPosition(RespawnPos);
			return;
		}
		appearanceResetFrame++;
	}
	else if(!IsDead){
		//拍終了
		if(IsBeatEnd){
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
			}
		}
	}
	BaseObjObject::Update(this->camera);
}

void TrainingDummy::Draw()
{
	if(IsDead) return;

	BaseObjObject::Draw();
}

void TrainingDummy::Finalize()
{
	BaseObjObject::Finalize();
}

void TrainingDummy::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsDead = true;
		IsDeadAudioOnce = true;
		RespawnPos = GetPosition();
		SetPosition(DeadPos);
	}
}
