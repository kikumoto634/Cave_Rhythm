#include "Walls.h"
#include "../../Collision/SphereCollider.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"

using namespace DirectX;

Walls::~Walls()
{
	//Finalize();
}

void Walls::Initialize(ObjModelManager* model, ObjModelManager* collider)
{
	BaseObjObject::Initialize(model);

	colliderModel = collider;
}

void Walls::Update(Camera *camera)
{
	this->camera = camera;

	IsDig = false;
	if(!IsAlive) return;
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();

	if(-13 <= distance && distance <= 13)		{
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
	else if(-13 > distance || distance > 13){
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

void Walls::Draw()
{
	if(!IsAlive) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void Walls::Finalize()
{
	BaseObjObject::Finalize();
}

void Walls::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;
	if(!IsHide) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsAlive = false;
		IsDig = true;
		digPosition = GetPosition();
		world.translation = {0,0,0};
		world.UpdateMatrix();
		collider->Update();
	}
}

