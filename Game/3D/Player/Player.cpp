#include "Player.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;


Player::~Player()
{
	//Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//入力
	input = Input::GetInstance();
	//武器
	weapon = new PlayerWeapon();
	weapon->Initialize("Slash", true);
	//攻撃モデル
	attackModel = new ObjModelManager();
	attackModel->CreateModel("human2");

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	//コライダーの追加
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,-0.2f,0,0}, radius));
	//当たり判定属性
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	//球コライダー取得
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;
	
	//Once用毎ループ初期化
	IsInputOnce = false;
	IsDamageSoundOnce = false;

	//入力処理
	if(MovementInput()){
		//入力確認
		IsInputOnce = true;
	}
	else if(AttackInput()){
		//入力確認
		IsInputOnce = true;
	}

	//拍終了(ゲーム全体の時間を測っているクラス(GameScene)からIsBeatEndOn()を呼び出しでtrueを取得)
	if(IsBeatEnd){
		//スケールイージング
		if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
			IsBeatEnd = false;
		}

		if(IsInputJudge){
			IsInputJudge = false;
			//移動
			if(IsMove){
				world.translation += movePosition;
				world.rotation = moveRotation;
				MoveModelSet();
				movePosition = {};
				IsMove = false;
			}
			//攻撃
			else if(IsAttack){
				weapon->Attack();
				AttackModelSet();
				IsAttack = false;
			}
		}
	}

	//移動制限
	world.translation.x = max(world.translation.x , -12.5f);
	world.translation.x = min(world.translation.x , 12.5f);
	world.translation.z = max(world.translation.z , -12.5f);
	world.translation.z = min(world.translation.z , 12.5f);

	//武器位置
	weapon->SetPosition(world.translation + offSetWeaponPos);
	weapon->SetRotation(moveRotation);

	//更新関数
	//ダメージ
	DamageUpdate();
	//コライダー更新
	collider->Update();
	//武器
	weapon->Update(this->camera);
	//ベース更新
	BaseObjObject::Update(this->camera);
}

void Player::Draw()
{
	weapon->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	weapon->Finalize();
	delete weapon;
	weapon = nullptr;

	delete attackModel;
	attackModel = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	//敵接触
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		Damage();
		camera->ShakeStart();
	}
}


void Player::JudgeUpdate(bool IsFlag)
{
	//正
	if(IsFlag){
		IsInputJudge = true;
		return;
	}
	//否
	IsInputJudge = false;
	return;
}

bool Player::DamageSound()
{
	if(IsDamage && IsDamageSoundOnce) return true;
	return false;
}



bool Player::MovementInput()
{
	//戻り値
	bool IsReturn = false;

	if(IsAttack) return false;

	//過去の位置取得
	oldPosition = GetPosition();
	//歩行
	if(input->Trigger(DIK_UP)){
		movePosition.z = 2.5f;
		moveRotation.y = 0;
		offSetWeaponPos = {0,0,2.5};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_DOWN)){
		movePosition.z = -2.5f;
		moveRotation.y = XMConvertToRadians(180);
		offSetWeaponPos = {0,0,-2.5};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_RIGHT)){
		movePosition.x = 2.5f;
		moveRotation.y = XMConvertToRadians(90);
		offSetWeaponPos = {2.5,0,0};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_LEFT)){
		movePosition.x = -2.5f;
		moveRotation.y = XMConvertToRadians(-90);
		offSetWeaponPos = {-2.5,0,0};
		IsReturn = true;
	}
	if(IsReturn) {
		//モデル識別
		IsModelJudge = false;
		//行動
		IsMove = true;
	}
	return IsReturn;
}

void Player::MoveModelSet()
{
	this->object->SetModel(model);
}

bool Player::AttackInput()
{
	//戻り値
	bool IsReturn = false;

	if(IsMove) return false;

	if(input->Trigger(DIK_Z)){
		IsReturn = true;
	}

	if(IsReturn) {
		//モデル識別
		IsModelJudge = true;
		//行動
		IsAttack = true;
	}
	return IsReturn;
}

void Player::AttackModelSet()
{
	this->object->SetModel(attackModel);
}

void Player::Damage()
{
	if(IsDamage) return;
	IsDamage = true;
	IsDamageSoundOnce = true;
	HP -= 1;
}

void Player::DamageUpdate()
{
	if(!IsDamage) return ;

	damageCurrentFrame += 1;

	//無敵時間内
	Vector4 color = (damageCurrentFrame % 6 == 1) ? color = {0.0f, 0.0f, 0.0f, 0.0f} : color = {1.0f, 0.0f, 0.0f, 1.0f};
	object->SetColor(color);

	//無敵時間終了
	if(damageCurrentFrame < DamageFrame) return;
	damageCurrentFrame = 0;
	object->SetColor({1.0f,1.0f,1.0f, 1.0f});
	IsDamage = false;
}
