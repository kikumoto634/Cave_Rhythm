#include "Coins.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"


void Coins::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	SetPosition(DeadPos);
	
	//コライダー
	ColliderSet();
}

void Coins::Update(Camera *camera)
{
	this->camera = camera;

	if(!isAlive_)return;

	//ビート処理
	if(IsBeatEnd){
		
		if(loatCount_ >= LostCountMax){
			loatCount_ = 0;
			isAlive_ = false;
		}

		loatCount_++;
		IsBeatEnd = false;
	}

	//コライダー更新
	collider->Update();
	BaseObjObject::Update(this->camera);
}

void Coins::Draw()
{
	if(!isAlive_) return;

	BaseObjObject::Draw();
}


void Coins::OnCollision(const CollisionInfo &info)
{
	if(!isAlive_) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		isAlive_ = false;
		isContactTrigger_ = true;
		isPopsPosibble_ = true;
		SetPosition(DeadPos);
	}
}


void Coins::Pop(Vector3 pos)
{
	BaseObjObject::Pop(pos);
	isAlive_ = true;
	loatCount_ = 0;
}


void Coins::ColliderSet()
{
	SetCollider(new SphereCollider(DirectX::XMVECTOR{0,SphereColliderRadius,0,0}, SphereColliderRadius));
	collider->SetAttribute(COLLISION_ATTR_ITEMS);
}
