#include "Player.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

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
	//死亡モデル
	deadModel = new ObjModelManager();
	deadModel->CreateModel("human3");

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 1.0f, 0.7f};

	//待機フラグ
	IsWait = true;

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

			//待機フラグ解除
			if(IsWait && !IsNextScene){
				IsWait = false;
			}
		}

		if(IsInputJudge){
			IsInputJudge = false;
			//移動
			if(IsMove){
				IsMoveEasing = true;
				IsMove = false;
				world.rotation = moveRotation;
				moveEasingPos = world.translation;
				MoveModelSet();
			}
			//攻撃
			else if(IsAttack){
				weapon->Attack();
				AttackModelSet();
				IsAttack = false;
			}
		}
	}

	//移動イージング
	if(IsMoveEasing){
		world.translation = Easing_Linear_Point2(moveEasingPos, movePosition, Time_OneWay(moveEasingFrame, MoveEasingMaxTime));

		if(moveEasingFrame >= 1.f){
			IsMoveEasing = false;
			world.translation = movePosition;
			moveEasingPos = {};
			movePosition = {};
			moveEasingFrame = 0;
		}
	}

	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = {RayDir.x, RayDir.y, RayDir.z, 0};
	RaycastHit raycastHit;
	if(IsMoveEasing){
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.1f;
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
			movePosition = OldPosition;
		}
		else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ENEMYS, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
			movePosition = OldPosition;
		}
	}

	//死亡
#ifdef _DEBUG
	if(input->Trigger(DIK_SPACE)){
		HP = 0;
	}
#endif // _DEBUG

	if(HP <= 0 && !IsDead){
		IsDead = true;
		IsDeadAudioOnce = true;
		this->object->SetModel(deadModel);
		//this->camera->ShakeStart();
	}

	//移動制限
	world.translation.x = max(world.translation.x , 15 * -2.f);
	world.translation.x = min(world.translation.x , 15 * 2.f);
	world.translation.z = max(world.translation.z , 15 * -2.f);
	world.translation.z = min(world.translation.z , 15 * 2.f);

	//武器位置
	weapon->SetPosition(world.translation + offSetWeaponPos);
	weapon->SetRotation(GetRotation());

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

	delete deadModel;
	deadModel = nullptr;

	delete attackModel;
	attackModel = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	if(IsWait) return;
	if(IsDead) return;

	//敵接触
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		Damage();
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

bool Player::GetIsDeadAudioOnce()
{
	if(IsDeadAudioOnce) {
		IsDeadAudioOnce = false;
		return true;
	}

	return false;
}


bool Player::MovementInput()
{
	//戻り値
	bool IsReturn = false;

	if(IsWait) return false;
	if(IsDead) return false;
	if(IsAttack) return false;
	if(IsMoveEasing) return false;

	//歩行
	if(input->Trigger(DIK_UP)){
		movePosition = Vector3{0.0f,0.0f,2.f};
		moveRotation.y = 0;
		offSetWeaponPos = {0,0,2.f};
		RayDir = {0,0,1};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_DOWN)){
		movePosition = Vector3{0.0f,0.0f,-2.f};
		moveRotation.y = XMConvertToRadians(180);
		offSetWeaponPos = {0,0,-2.f};
		RayDir = {0,0,-1};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_RIGHT)){
		movePosition = Vector3{2.f,0.0f,0.0f};
		moveRotation.y = XMConvertToRadians(90);
		offSetWeaponPos = {2.f,0,0};
		RayDir = {1,0,0};
		IsReturn = true;
	}
	else if(input->Trigger(DIK_LEFT)){
		movePosition = Vector3{-2.f,0.0f,0.0f};
		moveRotation.y = XMConvertToRadians(-90);
		offSetWeaponPos = {-2.f,0,0};
		RayDir = {-1,0,0};
		IsReturn = true;
	}
	if(IsReturn) {
		//モデル識別
		IsModelJudge = false;
		//行動
		IsMove = true;
		//移動後座標
		OldPosition = world.translation;
		movePosition += world.translation;
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

	if(IsWait) return false;
	if(IsDead) return false;
	if(IsMove) return false;

	if(input->Trigger(DIK_Z)){
		IsReturn = true;

		//出口
		if(IsExitOpen){
			IsNextScene = true;
			return false;
		}
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
	camera->ShakeStart(3);
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
