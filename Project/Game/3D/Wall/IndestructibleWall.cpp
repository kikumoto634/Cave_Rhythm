#include "IndestructibleWall.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

using namespace DirectX;

IndestructibleWall::~IndestructibleWall()
{
	//Finalize();
}

void IndestructibleWall::Initialize(ObjModelManager* model, ObjModelManager* collider)
{
	BaseObjObject::Initialize(model);

	isAlive_ = true;

	colliderModel = collider;
	ColliderInitialize();
}

void IndestructibleWall::Update(Camera *camera)
{
	this->camera = camera;

	IsReflect = false;
	if(!isAlive_) return;
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();

	if(IsCaveLight){
		if(-DrawingRange_Half <= distance && distance <= DrawingRange_Half){
			object->OnLighting();
		}
		else if(-DrawingRange_Half > distance || distance > DrawingRange_Half){
			object->OffLighting();
		}
	}
	else if(!IsCaveLight){
		object->OnLighting();
	}

	if(-DrawingRange <= distance && distance <= DrawingRange)		{
		IsHide = true;

		if(!IsCollision){
			
			ColliderSet();
			IsCollision = true;
		}
	}
	else if(-DrawingRange > distance || distance > DrawingRange){
		IsHide = false;

		if(IsCollision){
			ColliderRemove();
			IsCollision = false;
		}
	}
	

	if(!IsHide) return;
	BaseObjObject::Update(this->camera);
}

void IndestructibleWall::Draw()
{
	if(!isAlive_) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void IndestructibleWall::OnCollision(const CollisionInfo &info)
{
	if(!isAlive_) return;
	if(!IsHide) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){

		if(IsReflect)return;
		IsReflect = true;
	}
}

void IndestructibleWall::ColliderInitialize()
{
	//コライダー追加
	collider = new MeshCollider;
}

void IndestructibleWall::ColliderSet()
{
	//属性セット
	SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(colliderModel);
}

void IndestructibleWall::ColliderRemove()
{
	if(!collider) return;
	if(!IsCollision) return;

	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
	IsCollision = false;
}
