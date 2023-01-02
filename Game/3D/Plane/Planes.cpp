#include "Planes.h"

Planes::~Planes()
{
	Finalize();
}

void Planes::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//コライダー追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	//属性セット
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(model);
}

void Planes::Update(Camera *camera)
{
	this->camera = camera;

	//拍終わり時 && プレイヤー接触時
	if(IsBeatEnd && IsPlayerContact){
		
		//サイズ変更
		if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
			IsPlayerContact = false;
			IsBeatEnd = false;
		}
	}

	BaseObjObject::Update(this->camera);
}

void Planes::Draw()
{
	BaseObjObject::Draw();
}

void Planes::Finalize()
{
	BaseObjObject::Finalize();
}

void Planes::OnCollision(const CollisionInfo &info)
{
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsPlayerContact = true;
		object->SetColor({0.8f, 0.8f, 0.0f,1.0f});
	}	
}
