#include "Coins.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"


void Coins::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	SetPosition(DeadPos);
	
	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(DirectX::XMVECTOR{0,radius,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ITEMS);
}

void Coins::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsAlive)return;

	//ビート処理
	if(IsBeatEnd){
		
		if(lostBeat >= LostMaxBeat){
			lostBeat = 0;
			IsAlive = false;
			IsGet = false;
		}

		lostBeat++;
		IsBeatEnd = false;
	}

	//コライダー更新
	collider->Update();
	BaseObjObject::Update(this->camera);
}

void Coins::Draw()
{
	if(!IsAlive) return;

	BaseObjObject::Draw();
}


void Coins::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsAlive = false;
		IsGet = true;
		SetPosition(DeadPos);
	}
}

void Coins::Pop(Vector3 pos)
{
	SetPosition(pos);
	IsAlive = true;
	lostBeat = 0;
}

bool Coins::PopPossible()
{
	if(!IsAlive) return true;

	return false;
}

bool Coins::GetCoin()
{
	if(IsGet)	{
		IsGet = false;
		return true;
	}

	return false;
}
