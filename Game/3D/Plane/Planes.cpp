#include "Planes.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"

Planes::~Planes()
{
	//Finalize();
}

void Planes::Initialize(ObjModelManager *model)
{
	BaseObjObject::Initialize(model);
}

void Planes::Update(Camera *camera)
{
	this->camera = camera;
	if(!IsAlive) return;
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();

	if(-13 <= distance && distance <= 13)		{
		IsHide = true;

		if(!IsCollision){
			//�R���C�_�[�ǉ�
			MeshCollider* collider = new MeshCollider;
			SetCollider(collider);
			//�����Z�b�g
			collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
			collider->ConstructTriangles(model);
			IsCollision = true;
		}
	}
	else if(-13 > distance || distance > 13){
		IsHide = false;

		if(IsCollision){
			if(collider){
				//�R���W�����}�l�[�W���[����o�^����������
				CollisionManager::GetInstance()->RemoveCollider(collider);
			}
			IsCollision = false;
		}
	}
	
	if(!IsHide) return;

	//���I��莞
	if(IsBeatEnd){
		
		//�v���C���[�ڐG��
		if(IsPlayerContact){
			//�T�C�Y�ύX
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsPlayerContact = false;
				IsBeatEnd = false;
			}
		}
	}

	BaseObjObject::Update(this->camera);
}

void Planes::Draw()
{
	if(!IsAlive) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void Planes::Finalize()
{
	BaseObjObject::Finalize();
}

void Planes::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;
	if(!IsHide) return;
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsPlayerContact = true;
	}	
}

void Planes::PlaneColorChange(bool IsSwitch,  bool IsColorChange)
{
	if(!IsAlive) return;
	if(IsSwitch){
		if(IsColorChange){
			object->SetColor({0.0f, 0.8f, 0.0f,1.0f});
		}
		else if(!IsColorChange){
			object->SetColor({1.f,1.f,1.f,1.f});
		}
	}
	else if(!IsSwitch){
		if(IsColorChange){
			object->SetColor({1.f,1.f,1.f,1.f});
		}
		else if(!IsColorChange){
			object->SetColor({0.8f, 0.0f, 0.0f,1.0f});
		}
	}
}
