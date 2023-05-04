#include "BlueSlime.h"
#include "BlueSlimeStateManager.h"
#include "BlueSlimeState.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;
using namespace std;

void BlueSlime::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//拡縮最小値
	ScaleMin = {0.7f, 0.7f, 0.7f};

	state_ = new BlueSlimeStateManager();
	//state_->SetNextState(new IdelSkeltonState);
	state_->SetNextState(new PopBlueSlimeState);

    //コライダー
    ColliderInitialize();

    //パーティクル
    ParticleInitialize();
}

void BlueSlime::Update(Camera *camera, const Vector3& playerPos)
{
	this->camera = camera;
	playerPos_ = playerPos;

    state_->Update(this);

    if(isDead_) return;
	//距離に応じた更新
	DistanceUpdate();
	//アクション更新
	ActionUpdate();
	//ビート更新
	BeatUpdate();

	BaseObjObject::Update(camera);
}

void BlueSlime::Draw()
{
	if(isPosImposibble_ || isInvisible_ || isDead_) return;

	BaseObjObject::Draw();
}

void BlueSlime::ParticleDraw()
{
    state_->ParticleDraw();
}

void BlueSlime::Finalize()
{
    deadParticle_->Finalize();
    popParticle_->Finalize();
	
	delete state_;
	state_ = nullptr;

	BaseObjObject::Finalize();
}

void BlueSlime::OnCollision(const CollisionInfo &info)
{
    if(isDead_) return;
    if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
        isDead_ = true;
    }
	else if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		isDead_ = true;
	}
}

void BlueSlime::Pop(Vector3 pos)
{
    particlePos_ = pos;
	isPosImposibble_ = false;
}

bool BlueSlime::GetIsDeadTrigger()
{
    //Trigger
    if(isDeadTrigger_){
        isDeadTrigger_ = false;
        return true;
    }
    return false;
}



void BlueSlime::ActionUpdate()
{
    //拡縮
	if(isScaleChange_ && ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
		isScaleChange_ = false;
	}

    //コライダー
    if(collider){
        sphereCollider_->Update();
    }
}

void BlueSlime::BeatUpdate()
{
	if(!IsBeatEnd) return;
    IsBeatEnd = false;

    //拡縮
    isScaleChange_ = true;
}

void BlueSlime::DistanceUpdate()
{
	//距離
	Vector3 sub = playerPos_ - world.translation;
	distance_ = sub.length();

	//光計算
	if(isLightCal){
		//距離範囲内
		if(-DrawingRange_Half <= distance_ && distance_ <= DrawingRange_Half){
			object->OnLighting();
		}
		else if(-DrawingRange_Half > distance_ || distance_ > DrawingRange_Half){
			object->OffLighting();
		}
	}
	else if(!isLightCal){
		object->OnLighting();
	}

	//範囲が非表示
	if(-DrawingRange_Not <= distance_ && distance_ <= DrawingRange_Not)		{
		isInvisible_ = false;
	}
	else if(-DrawingRange_Not > distance_ || distance_ > DrawingRange_Not)	{
		isInvisible_ = true;
	}
}


void BlueSlime::ColliderInitialize()
{
    SetCollider(new SphereCollider(XMVECTOR{0,0,0,0}, colliderRadius_));
    collider->SetAttribute(COLLISION_ATTR_ENEMYS);
    //球コライダー取得
	sphereCollider_ = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider_);
    collider->Update();
}

void BlueSlime::ColliderSet()
{
    collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void BlueSlime::ColliderRemove()
{
    //コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
}


void BlueSlime::ParticleInitialize()
{
    deadParticle_ = make_unique<ParticleObject>();
    deadParticle_->Initialize();

    popParticle_ = make_unique<ParticleObject>();
    popParticle_->Initialize();
}

