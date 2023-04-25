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


	if(isDuplicateLimit_) return;
	
	//移動処理
	InputMovement();
}

void Player::InputMovement()
{
	//入力
	bool isLEFT = input_->Trigger(DIK_LEFT);
	bool isRIGHT = input_->Trigger(DIK_RIGHT);
	bool isUP = input_->Trigger(DIK_UP);
	bool isDOWN = input_->Trigger(DIK_DOWN);

	if(!isLEFT && !isRIGHT && !isUP && !isDOWN) return;

	//固有処理
	if(isLEFT){
		addVector3 = {-MoveLength,0.f,0.f};
		world.rotation.y = XMConvertToRadians(-90);
	}
	else if(isRIGHT){
		addVector3 = {MoveLength,0.f,0.f};
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(isUP){
		addVector3 = {0.f,0.f,MoveLength};
		world.rotation.y = XMConvertToRadians(0);
	}
	else if(isDOWN){
		addVector3 = {0.f,0.f,-MoveLength};
		world.rotation.y = XMConvertToRadians(180);
	}
	rayCastDir_ = {float(isRIGHT - isLEFT),0, float(isUP - isDOWN)};

	//共通
	isInput_ = true;
	weaponOffset_ = addVector3;
	isDuplicateLimit_ = true;
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
			state_ = State::Dig;
			return ;
		}
		else if(CollisionManager::GetInstance()->Raycast(ray_, COLLISION_ATTR_ENEMYS, &rayCastHit_, sphereCollider_->GetRadius() * 2.0f + adsDistance)){
			state_ = State::Attack;
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
			world.translation = easingEndPos;
			easigStartPos_ = {};
			easingEndPos = {};
			easingMoveTime = 0.f;
			state_ = State::None;
		}
		break;

	case Player::Dig:
		weapon_->Attack();
		state_ = State::None;
		break;

	case Player::Attack:
		weapon_->Attack();
		state_ = State::None;
		break;

	default:
		weapon_->Attack();
		state_ = State::None;
		break;
	}
}
