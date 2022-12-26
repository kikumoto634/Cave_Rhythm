#include "Planes.h"
#include "../../../Engine/math/Easing/Easing.h"

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

	if(IsScaleChange && IsPlayerContact){
		if(IsPlayerContact){
			float ease = -(cosf(3.14159265f * scaleTime) - 1.f)/2.f;
			scale = Easing_Linear_Point2(Vector3{ScaleMin,ScaleMax,ScaleMin}, Vector3{ScaleMax,ScaleMax,ScaleMax}, ease);
			SetScale(scale);
		
			if(scaleTime < 1.0f){
				scaleTime += 1.f/15;
			}
			else{
				scale = {ScaleMax, ScaleMax, ScaleMax};
				IsScaleChange = false;
				IsPlayerContact = false;
				scaleTime = 0.f;
			}
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
		object->SetColor({1.f, 1.f, 0.f});
	}	
}
