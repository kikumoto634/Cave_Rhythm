#include "GameManager.h"
#include "../../Engine/3D/ObjModelObject.h"

using namespace std;

void GameManager::Initialize()
{
	AudioInitialize();
	LightInitialize();
	SpriteInitialize();
}

void GameManager::Finalize()
{
	for(int i = 0; i < HpSpSize; i++){
		hpSp[i]->Finalize();
	}

	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Finalize();
	}
	comboSp->Finalize();

	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_coin[i]->Finalize();
	}
	coinSp->Finalize();

	delete lightGroup;
	lightGroup = nullptr;

	audio->Finalize();
}

void GameManager::ComboIncrement()
{
	comboNum += 1;

	//スプライト更新
	int hundred = comboNum/100;
	int ten = (comboNum - (hundred*100))/10;
	int one = (comboNum - (hundred*100) - (ten*10))/1;
	numberSp_combo[0]->SetTexNumber(hundred + TexNumberBegin);
	numberSp_combo[1]->SetTexNumber(ten + TexNumberBegin);
	numberSp_combo[2]->SetTexNumber(one + TexNumberBegin);
}

void GameManager::ComboReset()
{
	comboNum = 0;
	AudioPlay(1,0.7f);

	//スプライト更新
	for(int i = 0; i <NumberSpSize; i++){
		numberSp_combo[i]->SetTexNumber(TexNumberBegin);
	}
}

void GameManager::CoinIncrement()
{
	coinNum += 1;

	//スプライト更新
	int hundred = coinNum/100;
	int ten = (coinNum - (hundred*100))/10;
	int one = (coinNum - (hundred*100) - (ten*10))/1;
	numberSp_coin[0]->SetTexNumber(hundred + TexNumberBegin);
	numberSp_coin[1]->SetTexNumber(ten + TexNumberBegin);
	numberSp_coin[2]->SetTexNumber(one + TexNumberBegin);
}

void GameManager::InitializeSetHp(int _hp)
{
	hp = _hp;

	//体力
	HpSpSize = hp;
	DamageHpSpriteIndex = hp-1;
	for(int i = 0; i < HpSpSize; i++){
		std::unique_ptr<BaseSprites> Sp = make_unique<BaseSprites>();
		Sp->Initialize(14);
		Sp->SetPosition({float(900 + (i*80)), 55});
		Sp->SetSize({75,75});
		Sp->SetAnchorPoint({0.5f,0.5f});
		hpSp.push_back(move(Sp));
	}
}

void GameManager::HpDecrement()
{
	if(DamageHpSpriteIndex < 0)	return;

	hpSp[DamageHpSpriteIndex]->SetTexNumber(15);
	hpSp[DamageHpSpriteIndex]->SetSize({50,50});
	DamageHpSpriteIndex--;
}

void GameManager::AudioPlay(int number, float volume, bool loop)
{
	audio->PlayWave(number, volume, loop);
}

void GameManager::LightUpdate(bool IsPlayerShadowDead)
{
	if(!IsPlayerShadowAlive && !IsPlayerShadowDead){	
		//丸影
		lightGroup->SetCircleShadowDir(0, {circleShadowDir.x, circleShadowDir.y, circleShadowDir.z, 0});
		lightGroup->SetCircleShadowAtten(0, circleShadowAtten);
		lightGroup->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
	}

	if(IsPlayerShadowDead){
		IsPlayerShadowAlive = true;
		lightGroup->SetCircleShadowActive(0,false);
	}
	lightGroup->Update();
}

void GameManager::PlayerCircleShadowSet(Vector3 pos)
{
	//プレイヤー、丸影座標
	lightGroup->SetCircleShadowCasterPos(0, pos);
}

int GameManager::EnemyPopTurnCount()
{
	currentEnemyPopBeatTurn++;
	if(currentEnemyPopBeatTurn >= EnemyPopBeatTurn){
		currentEnemyPopBeatTurn = 0;
		return EnemyPopCreateNum;
	}

	return 0;
}

Vector2 GameManager::EnemyRandomPos(const int groundWidth, const float Plane_Size)
{
	Vector2 lpos;
	lpos.x = static_cast<float>(rand() % (groundWidth) - 5) * Plane_Size;
	lpos.y = static_cast<float>(rand() % (groundWidth) - 5) * Plane_Size;

	return lpos;
}

Vector2 GameManager::EnemyRandomDir(Vector2 pos)
{
	Vector2 ldir;
	//0:{0,1}, 1:{0,-1}, 2:{1,0}, 3:{-1,0}
	int random = rand()%4;	//0~3
	
	//奥左
	if(pos.x < 0 && pos.y < 0){
		//右向き
		if(random >= 1){
			ldir = {1,0};
		}
		//手前向き
		else if(random <= 2){
			ldir = {0,1};
		}
	}
	//奥右
	else if(pos.x > 0 && pos.y < 0){
		//左向き
		if(random >= 1){
			ldir = {-1,0};
		}
		//手前向き
		else if(random <= 2){
			ldir = {0,1};
		}
	}
	//手前右
	else if(pos.x > 0 && pos.y > 0){
		//左向き
		if(random >= 1){
			ldir = {-1,0};
		}
		//奥向き
		else if(random <= 2){
			ldir = {0,-1};
		}
	}
	//手前左
	else if(pos.x < 0 && pos.y > 0){
		//右向き
		if(random >= 1){
			ldir = {1,0};
		}
		//奥向き
		else if(random <= 2){
			ldir = {0,-1};
		}
	}
	//中心地
	else {
		//左
		if(random == 0){
			ldir = {-1,0};
		}
		//右
		else if(random == 1){
			ldir = {1,0};
		}
		//奥
		else if(random == 2){
			ldir = {0,-1};
		}
		//手前
		else if(random == 3){
			ldir = {0,1};
		}
	}

	return ldir;
}

void GameManager::SpriteUpdate()
{
	comboSp->Update();
	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Update();
	}

	coinSp->Update();
	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_coin[i]->Update();
	}

	if(IsHpScaleChange){
		if(hpSp[DamageHpSpriteIndex]->ScaleChange({75,75}, {50,50})){
			IsHpScaleChange = false;
		}
	}
	for(int i = 0; i < HpSpSize; i++){
		hpSp[i]->Update();
	}
}

void GameManager::SpriteDraw()
{
	comboSp->Draw();
	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Draw();
	}

	coinSp->Draw();
	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_coin[i]->Draw();
	}

	for(int i = 0; i < HpSpSize; i++){
		hpSp[i]->Draw();
	}
}

void GameManager::AudioInitialize()
{
	audio = Audio::GetInstance();
}

void GameManager::LightInitialize()
{
	//ライト
	lightGroup = LightGroup::Create();
	//色設定
	lightGroup->SetAmbientColor({0.15f, 0.15f, 0.15f});
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//丸影
	lightGroup->SetCircleShadowActive(0, true);
}

void GameManager::SpriteInitialize()
{
	//コンボテキスト
	comboSp = make_unique<BaseSprites>();
	comboSp->Initialize(2);
	comboSp->SetPosition({50,300});
	comboSp->SetSize({150,75});

	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i] = make_unique<BaseSprites>();
		numberSp_combo[i]->Initialize(TexNumberBegin + 0);
		numberSp_combo[i]->SetPosition({float(50+(i*50)),375});
		numberSp_combo[i]->SetSize({50,75});
	}

	//コインテキスト
	coinSp = make_unique<BaseSprites>();
	coinSp->Initialize(13);
	coinSp->SetPosition({50,175});
	coinSp->SetSize({75,75});

	//数字
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_coin[i] = make_unique<BaseSprites>();
		numberSp_coin[i]->Initialize(TexNumberBegin + 0);
		numberSp_coin[i]->SetPosition({float(125+(i*50)),175});
		numberSp_coin[i]->SetSize({50,75});
	}
}
