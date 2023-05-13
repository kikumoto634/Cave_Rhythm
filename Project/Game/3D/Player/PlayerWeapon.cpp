#include "PlayerWeapon.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "Easing.h"

using namespace DirectX;


void PlayerWeapon::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//半径文だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(ColliderOffSet, ColliderRadius));
	//当たり判定属性
	baseCollider_->SetAttribute(COLLISION_ATTR_WEAPONS);
	//球コライダー取得
	sphereCollider_ = dynamic_cast<SphereCollider*>(baseCollider_);
	assert(sphereCollider_);
}

void PlayerWeapon::Update(Camera *camera)
{
	if(!isAppear_) return;
	this->camera_ = camera;
	
	//生存フレーム
	if(aliveCurrentTime_ >= AliveTime) {
		aliveCurrentTime_ = 0;

		world_.translation = HidePos;
		BaseObjObject::Update(this->camera_);

		isAppear_ = false;
		return;
	}
	world_.scale = Easing_Linear_Point2(StartSize, EndSize,Time_OneWay(aliveCurrentTime_, AliveTime));

	//ベース更新
	BaseObjObject::Update(this->camera_);
}

void PlayerWeapon::Draw()
{
	if(!isAppear_) return;

	BaseObjObject::Draw();
}


void PlayerWeapon::OnCollision(const CollisionInfo &info)
{
	if(!isAppear_) return;

	unsigned short coll = info.collider->GetAttribute();
	unsigned short enemy = COLLISION_ATTR_ENEMYS;
	unsigned short dummy = COLLISION_ATTR_DUMMY;
	unsigned short land = COLLISION_ATTR_LANDSHAPE;

	if(coll == enemy || coll == dummy || coll == land){
		camera_->ShakeStart();
	}
}
