#include "Walls.h"
#include "../../Collision/SphereCollider.h"

using namespace DirectX;

Walls::~Walls()
{
	//Finalize();
}

void Walls::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	colliderModel = new ObjModelManager();
	colliderModel->CreateModel("GroundBlock2_Collider");

	//コライダー追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	//属性セット
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(colliderModel);
}

void Walls::Update(Camera *camera)
{
	this->camera = camera;
	IsDigSound = false;

	if(!IsAlive) return;
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();

	if(-20 <= distance && distance <= 20)		{
		IsHide = true;
	}
	else if(-20 > distance || distance > 20)	IsHide = false;
	
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
		IsDigSound = true;
		
		IsAlive = false;
		world.translation = {0,0,0};
		world.UpdateMatrix();
		collider->Update();
	}
}
