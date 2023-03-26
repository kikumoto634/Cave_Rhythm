#include "IndestructibleWall.h"
#include "../../Collision/SphereCollider.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"

using namespace DirectX;

IndestructibleWall::~IndestructibleWall()
{
	//Finalize();
}

void IndestructibleWall::Initialize(ObjModelManager* model, ObjModelManager* collider)
{
	BaseObjObject::Initialize(model);

	colliderModel = collider;

	//object->OffLighting();
}

void IndestructibleWall::Update(Camera *camera)
{
	this->camera = camera;

	if(!IsAlive) return;
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
			//コライダー追加
			MeshCollider* collider = new MeshCollider;
			SetCollider(collider);
			//属性セット
			collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
			collider->ConstructTriangles(colliderModel);
			IsCollision = true;
		}
	}
	else if(-DrawingRange > distance || distance > DrawingRange){
		IsHide = false;

		if(IsCollision){
			if(collider){
				//コリジョンマネージャーから登録を解除する
				CollisionManager::GetInstance()->RemoveCollider(collider);
			}
			IsCollision = false;
		}
	}
	

	if(!IsHide) return;
	BaseObjObject::Update(this->camera);
}

void IndestructibleWall::Draw()
{
	if(!IsAlive) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}


void IndestructibleWall::Finalize()
{
	BaseObjObject::Finalize();
}

void IndestructibleWall::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;
	if(!IsHide) return;
}

void IndestructibleWall::ColliderRemove()
{
	if(!IsCollision) return;

	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
	IsCollision = false;
}
