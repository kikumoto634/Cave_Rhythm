#include "Player.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Easing.h"

using namespace DirectX;

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//ポインタ
	//入力
	input_ = Input::GetInstance();
	//武器
	weapon_ = new PlayerWeapon();
	weapon_->Initialize("Slash", false);

	//拡縮最小値
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//コライダー
	SphereColliderSet();
}

void Player::Update(Camera *camera)
{
	this->camera = camera;

	//アクション更新
	ActionUpdate();
	//入力更新
	InputUpdate();
	//ビート更新
	BeatUpdate();

	//武器更新
	weapon_->SetPosition(world.translation + weaponOffset_);
	weapon_->SetRotation(GetRotation());
	weapon_->Update(this->camera);

	//ベース更新
	BaseObjObject::Update(this->camera);
}

void Player::Draw()
{
	weapon_->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	weapon_->Finalize();
	delete weapon_;
	weapon_ = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
}

void Player::SphereColliderSet()
{
	//コライダーの追加
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,-0.2f,0,0}, radius));
	//当たり判定属性
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	//球コライダー取得
	sphereCollider_ = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider_);
}

void Player::InputUpdate()
{
	isInput_ = false;

	//入力
	bool isLEFT = input_->Trigger(DIK_LEFT);
	bool isRIGHT = input_->Trigger(DIK_RIGHT);
	bool isUP = input_->Trigger(DIK_UP);
	bool isDOWN = input_->Trigger(DIK_DOWN);

	if(isLEFT){
		isInput_ = true;
		addVector3 = {-2.f,0.f,0.f};
		rayCastDir_ = {-1,0,0};
		world.rotation.y = XMConvertToRadians(-90);

		weaponOffset_ = addVector3;
	}
	else if(isRIGHT){
		isInput_ = true;
		addVector3 = {2.f,0.f,0.f};
		rayCastDir_ = {1,0,0};
		world.rotation.y = XMConvertToRadians(90);

		weaponOffset_ = addVector3;
	}
	else if(isUP){
		isInput_ = true;
		addVector3 = {0.f,0.f,2.f};
		rayCastDir_ = {0,0,1};
		world.rotation.y = XMConvertToRadians(0);

		weaponOffset_ = addVector3;
	}
	else if(isDOWN){
		isInput_ = true;
		addVector3 = {0.f,0.f,-2.f};
		rayCastDir_ = {0,0,-1};
		world.rotation.y = XMConvertToRadians(180);

		weaponOffset_ = addVector3;
	}
}

void Player::BeatUpdate()
{
	if(!IsBeatEnd) return;

	//スケール
	if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
		IsBeatEnd = false;
	}
}

void Player::ActionUpdate()
{
	//入力時判定が成功した場合
	if(isInputJudge_){
		isInputJudge_ = false;

		ray_.start = sphereCollider_->center;
		ray_.start.m128_f32[1] += sphereCollider_->GetRadius();
		ray_.dir = {rayCastDir_.x, rayCastDir_.y, rayCastDir_.z};

		RaycastHit rayCastHit_;
		//壁判定
		if(CollisionManager::GetInstance()->Raycast(ray_, COLLISION_ATTR_LANDSHAPE, &rayCastHit_, sphereCollider_->GetRadius() * 2.0f + adsDistance)){
			weapon_->Attack();
			return ;
		}

		//移動
		state_ = State::Move;
		easigStartPos_ = world.translation;
		easingEndPos = world.translation + addVector3;
	}


	//状態処理
	
	switch (state_)
	{
	case Player::None:
		break;

	case Player::Move:
		//移動処理
		world.translation = Easing_Linear_Point2(easigStartPos_, easingEndPos, Time_OneWay(easingMoveTime, EasingMoveTimeMax));

		//移動完了時
		if(easingMoveTime >= 1.0f){
			state_ = State::None;
			world.translation = easingEndPos;
			easigStartPos_ = {};
			easingEndPos = {};
			easingMoveTime = 0.f;
		}
		break;

	default:
		break;
	}
}
