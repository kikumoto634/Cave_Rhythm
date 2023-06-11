#include "Player.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Easing.h"
#include "PlayerStateManager.h"
#include "PlayerState.h"

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

	//攻撃モデル
	attackModel_ = new ObjModelManager();
	attackModel_->CreateModel("human2");
	//死亡モデル
	deadModel_ = new ObjModelManager();
	deadModel_->CreateModel("human3");

	//拡縮最小値
	scaleMin_ = SizeMin;

	//状態
	state_ = PlayerStateManager::GetInstance();
	state_->SetNextState(new IdelPlayerState);

	//生存
	isAlive_ = true;

	//コライダー
	SphereColliderSet();
}

void Player::Update(Camera *camera)
{
	this->camera_ = camera;

	//アクション更新
	ActionUpdate();
	state_->Update(this);
	//入力更新
	InputUpdate();
	//ビート更新
	BeatUpdate();
	//ダメージ
	DamageUpdate();


	if(input_->Trigger(DIK_SPACE)){
		//state_->SetNextState(new DeadPlayerState);

		isDamage_ = true;
		//hp_ = 0;
	}
#ifdef _DEBUG
	if(input_->Trigger(DIK_Z)){
		state_->SetNextState(new AttackPlayerState);
	}

#endif // _DEBUG


	//武器更新
	weapon_->Pop(world_.translation + weaponOffset_);
	weapon_->SetRotation(GetRotation());
	weapon_->Update(this->camera_);

	//ベース更新
	BaseObjObject::Update(this->camera_);
}

void Player::Draw()
{
	weapon_->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	delete attackModel_;
	attackModel_ = nullptr;
	delete deadModel_;
	deadModel_ = nullptr;

	weapon_->Finalize();
	delete weapon_;
	weapon_ = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		ContactUpdate();
	}
}


bool Player::GetIsDamage()
{
	if(!isAlive_) return false;
	//(サウンド管理をするのでトリガー)
	if(damageFrame_ == 0 && isDamage_){
		return true;
	}
	return false;
}

bool Player::GetIsDead()
{
	//(サウンド管理をするのでトリガー)
	if(damageFrame_ == 0 && !isAlive_){
		return true;
	}
	return false;
}

void Player::SphereColliderSet()
{
	//コライダーの追加
	SetCollider(new SphereCollider(SphereColliderOffSet, SphereColliderRadius));
	//当たり判定属性
	baseCollider_->SetAttribute(COLLISION_ATTR_ALLIES);
	//球コライダー取得
	sphereCollider_ = dynamic_cast<SphereCollider*>(baseCollider_);
	assert(sphereCollider_);
}

void Player::ColliderRemove()
{
	//コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(baseCollider_);
}

void Player::InputUpdate()
{
	isInput_ = false;

	//死亡
	if(!isAlive_) return;
	//シーン遷移
	if(isNextScene_) return ;
	//重複
	if(isDuplicateLimit_) return;
	//イベント中
	if(isEvent_) return;
	
	//移動処理
	InputMovement();
	//扉解放
	InputDecision();
}

void Player::ContactUpdate()
{
	isDamage_ = true;
}

void Player::InputMovement()
{
	//入力
	bool isLEFT  = (input_->Trigger(DIK_LEFT)  /*|| input_->Trigger(DIK_A)*/);
	bool isRIGHT = (input_->Trigger(DIK_RIGHT) /*|| input_->Trigger(DIK_D)*/);
	bool isUP    = (input_->Trigger(DIK_UP)    /*|| input_->Trigger(DIK_W)*/);
	bool isDOWN  = (input_->Trigger(DIK_DOWN)  /*|| input_->Trigger(DIK_S)*/);

	if(!isLEFT && !isRIGHT && !isUP && !isDOWN) return;

	//固有処理
	if(isLEFT){
		addVector3_ = LeftMove.moveAdd;
		world_.rotation.y = LeftMove.Rot;
	}
	else if(isRIGHT){
		addVector3_ = RightMove.moveAdd;
		world_.rotation.y = RightMove.Rot;
	}
	else if(isUP){
		addVector3_ = UpMove.moveAdd;
		world_.rotation.y = UpMove.Rot;
	}
	else if(isDOWN){
		addVector3_ = DownMove.moveAdd;
		world_.rotation.y = DownMove.Rot;
	}
	rayCastDir_ = {float(isRIGHT - isLEFT),0, float(isUP - isDOWN)};

	//共通
	isInput_ = true;
	weaponOffset_ = addVector3_;
	isDuplicateLimit_ = true;
}

void Player::InputDecision()
{
	bool isZ = input_->Trigger(DIK_Z);

	if(!isZ) return;
	if(!isExitOpen_) return;

	isNextScene_ = true;
}

void Player::BeatUpdate()
{
	if(!isBeatEnd_) return;

	//スケール
	if(ScaleChange(scaleMax_, scaleMin_, ScaleEndTime)){
		isBeatEnd_ = false;
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
			blockBreakPos_ = Vector2(world_.translation.x, world_.translation.z) + Vector2(rayCastDir_.x*MoveLength, rayCastDir_.z*MoveLength);
			state_->SetNextState(new DigPlayerState);
			return ;
		}
		else if(CollisionManager::GetInstance()->Raycast(ray_, COLLISION_ATTR_ENEMYS, &rayCastHit_, sphereCollider_->GetRadius() * 2.0f + adsDistance)){
			state_->SetNextState(new AttackPlayerState);
			return ;
		}

		//移動
		state_->SetNextState(new MovePlayerState);
	}
}

void Player::DamageUpdate()
{
	if(!isDamage_) return;
	if(!isAlive_) return;

	if(damageFrame_ == 0){
		hp_-= 1;
		
		//死亡判定
		if(hp_ <= 0){
			state_->SetNextState(new DeadPlayerState);

			isDamage_ = false;
			isAlive_ = false;
			return ;
		}

		camera_->ShakeStart(ShakeValue);
	}

	//無敵時間内
	Vector4 color = (damageFrame_ % damageFrameInterval == 1) ? color = Damage1Color : color = Damage2Color;
	object_->SetColor(color);
	damageFrame_ ++;

	//無敵時間終了
	if(damageFrame_ < DamageFrameMax) return;
	damageFrame_ = 0;
	object_->SetColor(NormalColor);
	isDamage_ = false;
}
