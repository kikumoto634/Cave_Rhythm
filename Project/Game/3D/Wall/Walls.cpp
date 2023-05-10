#include "Walls.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

using namespace DirectX;

Walls::~Walls()
{
}

void Walls::Initialize(ObjModelManager* model, ObjModelManager* collider)
{
	BaseObjObject::Initialize(model);

	isAlive_ = true;

	colliderModel = collider;
	ColliderInitialize();
}

void Walls::Update(Camera *camera)
{
	this->camera = camera;

	IsDig = false;
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

void Walls::Draw()
{
	if(!isAlive_) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void Walls::OnCollision(const CollisionInfo &info)
{
	if(!isAlive_) return;
	if(!IsHide) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		isAlive_ = false;
		IsDig = true;
		digPosition = GetPosition();
		world.translation = {0,0,0};
		world.UpdateMatrix();
		collider->Update();
	}
}

void Walls::ColliderInitialize()
{
	//コライダー追加
	collider = new MeshCollider;
}

void Walls::ColliderSet()
{
	SetCollider(collider);
	//属性セット
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(colliderModel);
}

void Walls::ColliderRemove()
{
	if(!collider) return;
	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
}

