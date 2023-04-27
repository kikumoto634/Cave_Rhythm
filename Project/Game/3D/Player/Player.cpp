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

	//拡縮最小値
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//状態
	state_ = PlayerStateManager::GetInstance();
	
	state_->SetNextState(new IdelPlayerState);

	//コライダー
	SphereColliderSet();
}

void Player::Update(Camera *camera)
{
	this->camera = camera;

	//アクション更新
	ActionUpdate();
	state_->Update(this);
	//入力更新
	InputUpdate();
	//ビート更新
	BeatUpdate();

	//ダメージ
	DamageUpdate();
	//死亡
	DeadUpdate();
#ifdef _DEBUG
	if(input_->Trigger(DIK_SPACE)){
		//isDead_ = true;
		isDamage_ = true;
	}
#endif // _DEBUG


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
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		isDamage_ = true;
	}
}


bool Player::GetIsDamage()
{
	if(isDead_) return false;
	//(サウンド管理をするのでトリガー)
	if(damageFrame_ == 0 && isDamage_){
		return true;
	}
	return false;
}

bool Player::GetIsDead()
{
	//(サウンド管理をするのでトリガー)
	if(damageFrame_ == 0 && isDead_){
		return true;
	}
	return false;
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

	//死亡
	if(isDead_) return;
	//シーン遷移
	if(isNextScene_) return ;
	//重複
	if(isDuplicateLimit_) return;
	
	//移動処理
	InputMovement();
	//扉解放
	InputDecision();
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
		addVector3_ = {-MoveLength,0.f,0.f};
		world.rotation.y = XMConvertToRadians(-90);
	}
	else if(isRIGHT){
		addVector3_ = {MoveLength,0.f,0.f};
		world.rotation.y = XMConvertToRadians(90);
	}
	else if(isUP){
		addVector3_ = {0.f,0.f,MoveLength};
		world.rotation.y = XMConvertToRadians(0);
	}
	else if(isDOWN){
		addVector3_ = {0.f,0.f,-MoveLength};
		world.rotation.y = XMConvertToRadians(180);
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
			state_->SetNextState(new DigPlayerState);
			return ;
		}
		else if(CollisionManager::GetInstance()->Raycast(ray_, COLLISION_ATTR_ENEMYS, &rayCastHit_, sphereCollider_->GetRadius() * 2.0f + adsDistance)){
			state_->SetNextState(new AttackPlayerState);
			return ;
		}

		//移動
		state_->SetNextState(new MovePlayerState);
		easigStartPos_ = world.translation;
		easingEndPos_ = world.translation + addVector3_;
	}
}

void Player::DamageUpdate(){
	
	if(!isDamage_) return;
	if(isDead_) return;

	//初回のみ
	if(damageFrame_ == 0){
		hp_-= 1;
		
		//死亡判定
		if(hp_ <= 0){
			isDamage_ = false;
			isDead_ = true;
			return ;
		}

		camera->ShakeStart(3);
	}

	//無敵時間内
	Vector4 color = (damageFrame_ % 6 == 1) ? color = {0.0f, 0.0f, 0.0f, 0.0f} : color = {1.0f, 0.0f, 0.0f, 1.0f};
	object->SetColor(color);
	damageFrame_ ++;


	//無敵時間終了
	if(damageFrame_ < DamageFrameMax) return;
	damageFrame_ = 0;
	object->SetColor({1.0f,1.0f,1.0f, 1.0f});
	isDamage_ = false;
}

void Player::DeadUpdate()
{
	if(!isDead_) return;

	//初回
	if(damageFrame_ == 0){
		camera->ShakeStart(3);
	}

	damageFrame_++;

	if(damageFrame_ < DamageFrameMax) return;
	damageFrame_ = 0;
}

