#include "Planes.h"
#include "CollisionManager.h"


void Planes::Initialize(ObjModelManager *model)
{
	BaseObjObject::Initialize(model);

	isAlive_ = true;

	object->OffLighting();
	ColliderInitialize();
}

void Planes::Update(Camera *camera)
{
	this->camera = camera;
	if(!isAlive_) return;
	//距離
	DistanceUpdate();
	
	if(!IsHide) return;

	//拍終わり時
	if(IsBeatEnd){
		
		//プレイヤー接触時
		if(IsPlayerContact){
			//サイズ変更
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
	if(!isAlive_) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void Planes::OnCollision(const CollisionInfo &info)
{
	if(!isAlive_) return;
	if(!IsHide) return;
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsPlayerContact = true;
	}	
}


void Planes::DistanceUpdate()
{
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

	if(-DrawingRange_Not <= distance && distance <= DrawingRange_Not)		{
		IsHide = true;

		if(!IsCollision){
			
			ColliderSet();
			IsCollision = true;
		}
	}
	else if(-DrawingRange_Not > distance || distance > DrawingRange_Not){
		IsHide = false;

		if(IsCollision){
			if(collider){
				ColliderRemove();
			}
			IsCollision = false;
		}
	}
}


void Planes::ColliderInitialize()
{
	//コライダー追加
	collider = new MeshCollider;
}

void Planes::ColliderSet()
{
	//属性セット
	SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(model);
}

void Planes::ColliderRemove()
{
	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
}


void Planes::PlaneColorChange(bool IsSwitch,  bool IsColorChange)
{
	if(!isAlive_) return;
	if(IsSwitch){
		if(IsColorChange){
			object->SetColor(GreenColor);
		}
		else if(!IsColorChange){
			object->SetColor(IniColor);
		}
	}
	else if(!IsSwitch){
		if(IsColorChange){
			object->SetColor(IniColor);
		}
		else if(!IsColorChange){
			object->SetColor(RedColor);
		}
	}
}
