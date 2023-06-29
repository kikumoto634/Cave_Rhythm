#include "Botton.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Easing.h"

Botton::~Botton()
{
	audio_->Finalize();
}

void Botton::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	isAlive_ = true;

	ColliderInitialize();

	audio_ = Audio::GetInstance();
}

void Botton::Update(Camera *camera)
{
	this->camera_ = camera;

	if(!isAlive_) return;
	Vector3 pos = playerPos_ - world_.translation;
	distance_ = pos.length();

	if(isLightCal_){
		if(-DrawingRange_Half <= distance_ && distance_ <= DrawingRange_Half){
			object_->OnLighting();
		}
		else if(-DrawingRange_Half > distance_ || distance_ > DrawingRange_Half){
			object_->OffLighting();
		}
	}
	else if(!isLightCal_){
		object_->OnLighting();
	}

	if(-DrawingRange <= distance_ && distance_ <= DrawingRange)		{
		isHide_ = true;

		if(!isCollision_){
			ColliderSet();
			isCollision_ = true;
		}
	}
	else if(-DrawingRange > distance_ || distance_ > DrawingRange){
		isHide_ = false;

		if(isCollision_){
			ColliderRemove();
			isCollision_ = false;
		}
	}
	
	ReturnButton();

	if(!isHide_) return;
	BaseObjObject::Update(this->camera_);
}

void Botton::Draw()
{
	if(!isAlive_) return;
	if(!isHide_) return;
	BaseObjObject::Draw();
}

void Botton::OnCollision(const CollisionInfo &info)
{
	if(!isAlive_) return;
	if(!isHide_) return;	

	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		ContactUpdate();
	}
}

void Botton::ContactUpdate()
{
	if(isPush_) return;
	audio_->PlayWave(push_audio.number, push_audio.volume);
	isPush_ = true;

	Vector3 lpos = GetPosition();
	lpos.y += PushDownPosY;
	SetPosition(lpos);

	RhythmChange();
}

void Botton::ReturnButton()
{
	if(!isPush_)return;
	Time_OneWay(intervalFrame, IntervalTime);

	if(intervalFrame >= 1.0f){
		intervalFrame = 0;
		audio_->PlayWave(push_audio.number, push_audio.volume);
		isPush_ = false;

		Vector3 lpos = GetPosition();
		lpos.y -= PushDownPosY;
		SetPosition(lpos);

		RhythmNormalChange();
	}
}

void Botton::RhythmNormalChange()
{
	audio_->StopWave(bpm90Game_audio.number);
	audio_->StopWave(bpm150Game_audio.number);
	rhythm_->BPMNormalSet();
	audio_->PlayWave(bpm120Game_audio.number, bpm120Game_audio.volume, true);
}

void Botton::ColliderInitialize()
{
	//コライダー追加
	collider_ = new MeshCollider;
}

void Botton::ColliderSet()
{
	SetCollider(collider_);
	//属性セット
	collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider_->ConstructTriangles(model_);
}

void Botton::ColliderRemove()
{
	if(!collider_) return;
	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider_);
}

void Botton::RhythmChange()	{}
