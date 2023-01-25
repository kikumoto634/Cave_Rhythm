#include "Coins.h"
#include "../../Collision/SphereCollider.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

Coins::~Coins()
{
}

void Coins::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	SetPosition(DeadPos);
	

	//�R���C�_�[
	float radius = 0.6f;
	SetCollider(new SphereCollider(DirectX::XMVECTOR{0,radius,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ITEMS);
}

void Coins::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsAlive)return;

	if(IsBeatEnd){
		
		if(lostBeat >= LostMaxBeat){
			lostBeat = 0;
			IsAlive = false;
			IsGet = false;
		}

		lostBeat++;
		IsBeatEnd = false;
	}

	//�R���C�_�[�X�V
	collider->Update();
	BaseObjObject::Update(this->camera);
}

void Coins::Draw()
{
	if(!IsAlive) return;

	BaseObjObject::Draw();
}

void Coins::Finalize()
{
	BaseObjObject::Finalize();
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
