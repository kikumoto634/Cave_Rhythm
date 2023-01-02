#include "Planes.h"

Planes::~Planes()
{
	Finalize();
}

void Planes::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//�R���C�_�[�ǉ�
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	//�����Z�b�g
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(model);
}

void Planes::Update(Camera *camera)
{
	this->camera = camera;

	//���I��莞 && �v���C���[�ڐG��
	if(IsBeatEnd && IsPlayerContact){
		
		//�T�C�Y�ύX
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
